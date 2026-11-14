/*
 *  Copyright (C) 2019 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

#include "x509-parser.h"

#ifdef ERROR_TRACE_ENABLE
#define ERROR_TRACE_APPEND(x) do {			    \
	       extern int printf(const char *format, ...);  \
	       printf("%05d", (x));			    \
	} while (0);
#else
#define ERROR_TRACE_APPEND(x)
#endif



static int bufs_differ(const u8 *b1, const u8 *b2, u16 n)
{
	int ret = 0;
	u16 i = 0;

	
	for (i = 0; i < n; i++) {
		if(b1[i] != b2[i]) {
			ret = 1;
			break;
		}
	}

	return ret;
}

/*
 * Helper for ASN.1 DER identifier field parsing, which extracts the tag number
 * when it is not encoded on a single byte, i.e. when the first encountered byte
 * for the field is 0x1f. The function takes as parameter the buffer following
 * this 0x1f byte and, upon success, extracts the tag value.
 *
 * In our implementation, tag numbers are limited by the return type used for
 * the parameter (32 bit unsigned integers). In practice we allow tag encoded
 * on 4 bytes, i.e. with a final value of 4 * 7 bits, i.e. 28 bits. This is
 * considered largely sufficient in the context of X.509 certificates (which is
 * validated by our tests).
 *
 * Note that the function does verify that extracted tag is indeed >= 0x1f.
 */

static int _extract_complex_tag(const u8 *buf, u16 len, u32 *tag_num, u16 *eaten)
{
	u16 rbytes;
	u32 t = 0;
	int ret;

	if ((len == 0) || (buf == NULL)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	if (len > 4) {
		len = 4;
	}

	
	for (rbytes = 0; rbytes < len; rbytes++) {
		u32 tmp1, tmp2;
		
		
		
		tmp1 = (t << (u32)7);
		tmp2 = ((u32)buf[rbytes] & (u32)0x7f);
		
		
		t = tmp1 + tmp2;
		
		

		if ((buf[rbytes] & 0x80) == 0) {
			break;
		}
	}

	/* Check if we left the loop w/o finding tag's end */
	if (rbytes == len) {
		
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	if (t < 0x1f) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*tag_num = t;
	*eaten = rbytes + 1;

	ret = 0;

out:
	return ret;
}


typedef enum {
	CLASS_UNIVERSAL = 0x00,
	CLASS_APPLICATION = 0x01,
	CLASS_CONTEXT_SPECIFIC = 0x02,
	CLASS_PRIVATE = 0x03
} tag_class;
/*
 * Parse beginning of current buffer to get the identifier parameters (class,
 * P/C flag and tag number). On success, the amount of bytes eaten to extract
 * those information is returned in 'eaten' parameter, which is guaranteed to
 * be lower or equal than 'len' parameter. On error, a non-zero negative value
 * is returned.
 *
 * Note: tags numbers are limited by the return type used for the parameter
 * (32 bit unsigned integer). In practice, this allows tag encoded on 4 bytes,
 * i.e. 4 x 7 bits, i.e. 28 bits. This is considered largely sufficient in
 * the context of X.509 certificates. An error is returned if a tag number
 * higher than that is found.
 */

static int get_identifier(const u8 *buf, u16 len,
			  tag_class *cls, u8 *prim, u32 *tag_num, u16 *eaten)
{
	int ret;
	u32 t;
	u16 rbytes = 0;
	u8 p;
	tag_class c;

	if (buf == NULL) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/*
	 * First byte (if available) will give us the class and P/C, and also
	 * tells us (based on the value of the 6 LSB of the bytes) if the tag
	 * number is stored on additional bytes.
	 */
	if (len == 0) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* See 8.1.2.3 */
	c = (buf[0] >> 6) & 0x03; /* Extract class from 2 MSB */
	p = (buf[0] >> 5) & 0x01; /* Extract P/C bit */
	t = buf[0] & 0x1f;        /* Extract tag number from 6 LSB */
	rbytes = 1;

	/*
	 * Check if we know given class (see Table 1 from 8.1.2.2). In practice,
	 * there is no way to end up in default case, because 'c' has at most
	 * its two MSB set (see above).
	 */
	switch (c) {
	case CLASS_UNIVERSAL:
	case CLASS_APPLICATION:
	case CLASS_CONTEXT_SPECIFIC:
	case CLASS_PRIVATE:
		break;
	default:
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
		break;
	}

	/*
	 * If the tag number (6 LSB of first byte) we already extracted is less
	 * than 0x1f, then it directly represents the tag number (which is <=
	 * 30) and our work is over. Otherwise (t == 0x1f) the real tag number
	 * is encoded on multiple bytes following the first byte. Note that we
	 * limit ourselves to tag encoded on less than 28 bits, i.e. only accept
	 * at most 4 bytes (only 7 LSB of each byte will count because MSB tells
	 * if this is the last).
	 */
	if (t == 0x1f) {
		u16 tag_len = 0;

		
		ret = _extract_complex_tag(buf + rbytes, len - rbytes,
					   &t, &tag_len);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		rbytes += tag_len;
	}

	/* Export what we extracted to caller */
	*cls = c;
	*prim = p;
	*tag_num = t;
	*eaten = rbytes;

	ret = 0;

out:
	return ret;
}

/*
 * Parse beginning of current buffer to get the length parameter. Input buffer
 * 'buf' has size 'len'. On success, 0 is returned, advertised length is
 * returned in 'adv_len' and the number of bytes used for the encoding of the
 * length is returned in 'eaten'.
 */

static int get_length(const u8 *buf, u16 len, u16 *adv_len, u16 *eaten)
{
	u16 l, rbytes = 0;
	u8 len_len, b0;
	int ret;

	if (buf == NULL) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	if (len == 0) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	
	b0 = buf[0];

	/* Single byte length (i.e. definitive form, on one byte)? */
	if ((b0 & 0x80) == 0) {
		l = b0 & 0x7f;
		

		/*
		 * Adding 1 below to take into account the byte that
		 * encode the length is possible because l does not
		 * have its MSB set, i.e. is less than or equal to
		 * 127.
		 */
		if ((l + 1) > len) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		/* Advertised length looks ok */
		*eaten = 1;
		*adv_len = l;
		
		ret = 0;
		goto out;
	}

	/*
	 * DER requires the definitive form for the length, i.e. that
	 * first byte of the length field is not 0x80. At that point,
	 * we already know that MSB of the byte is 1.
	 */
	if (b0 == 0x80) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/*
	 * We now know that the long form of the length is used. Let's
	 * extract how many bytes are used to encode the length.
	 */
	len_len = b0 & 0x7f;
	
	rbytes += 1;
	

	/*
	 * We first check that given length for the length field is not
	 * more than the size of the buffer (including the first byte
	 * encoding the length of that length). Note that we can do
	 * the addition below because MSB of len_len is 0.
	 */
	if ((len_len + 1) > len) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* Now that we have length's length, let's now extract its value */
	switch (len_len) {
	case 0: /* Not acceptable */
		/* Length's length cannot be 0 */
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
		break;

	case 1: /* Length values in [ 128, 255 ] */
		/* assert \valid_read(buf + 1); */
		l = buf[1];
		if (l <= 127) {
			/*
			 * For such a length value, the compact encoding
			 * (definitive form) should have been used.
			 */
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}
		
		rbytes += 1;
		break;

	case 2: /* Length values in [ 256, 65535 ] */
		/* assert \valid_read(buf + (1 .. 2)); */
		l = (((u16)buf[1]) * 256) + buf[2];
		if (l <= 0xff) {
			/* Why 2 bytes if most significant is 0? */
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}
		
		rbytes += 2;
		break;

	default: /* Not acceptable */
		/*
		 * Length cannot be encoded on more than two bytes (we
		 * have an *intentional* internal limitation for
		 * all ASN.1 DER structures set to 65535 bytes (all
		 * our lengths are u16)
		 */
		 ret = -__LINE__;
		 ERROR_TRACE_APPEND(__LINE__);
		 goto out;
		 break;
	}

	
	
	if ((len - rbytes) < l) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	
	
	*eaten = rbytes;
	*adv_len = l;

	ret = 0;

out:
	return ret;
}


static int _parse_arc(const u8 *buf, u16 len, u32 *arc_val, u16 *eaten)
{
	u16 rbytes;
	u32 av = 0;
	int ret;

	if ((len == 0) || (buf == NULL)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/*
	 * RFC 5280 has "There is no maximum size for OIDs. This specification
	 * mandates support for OIDs that have arc elements with values that
	 * are less than 2^28, that is, they MUST be between 0 and 268,435,455,
	 * inclusive. This allows each arc element to be represented within a
	 * single 32-bit word." For that reason, we just leave if we end up
	 * encountering more than 4 bytes here.
	 */
	if (len > 4) {
		len = 4;
	}

	
	for (rbytes = 0; rbytes < len; rbytes++) {
		u32 tmp1, tmp2;

		
		
		

		tmp1 = (av << (u32)7);
		
		tmp2 = ((u32)buf[rbytes] & (u32)0x7f);
		
		av = tmp1 + tmp2;
		
		

		if ((buf[rbytes] & 0x80) == 0) {
			break;
		}
	}

	if (rbytes >= len) {
		
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*arc_val = av;
	*eaten = rbytes + 1;

	ret = 0;

out:
	return ret;
}

/*
 * All DER-encoded elements are basically TLV structures (or identifier octets,
 * length octets, contents octets). This function parses the T and L elements
 * from given buffer and verifies the advertised length for the value (content
 * octets) does not overflow outside of the buffer. Additionally, the expected
 * class and type for the tag are verified. On success, the size of parsed
 * elements (class, type and length) are returned in 'parsed' and the size of
 * content octets are returned in 'content_len'.
 *
 * Note that the function does not parse the content of the encoded value, i.e.
 * the 'content_len' bytes in the buffer after the 'parsed' (TL header) ones. It
 * only guarantees that they are in the buffer.
 *
 * This function is critical for the security of the module.
 */

static int parse_id_len(const u8 *buf, u16 len, tag_class exp_class,
			u32 exp_type, u16 *parsed, u16 *content_len)
{
	tag_class c = 0;
	u8 p;
	u32 t = 0;
	u16 cur_parsed = 0;
	u16 grabbed;
	u16 adv_len = 0;
	int ret;

	if (buf == NULL) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* We voluntarily limit the size of the buffers we accept */
	if (len > ASN1_MAX_BUFFER_SIZE) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* Get the first part of the encoding, i.e. the identifier */
	ret = get_identifier(buf, len, &c, &p, &t, &cur_parsed);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}
	

	/*
	 * Now, verify we are indeed dealing with an element of
	 * given type ...
	 */
	if (t != exp_type) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* ... and class. */
	if (c != exp_class) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	grabbed = cur_parsed;
	
	len -= cur_parsed;
	buf += cur_parsed;

	/* Get the second part of the encoding, i.e. the length */
	ret = get_length(buf, len, &adv_len, &cur_parsed);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}
	

	grabbed += cur_parsed;
	
	len -= cur_parsed;
	buf += cur_parsed;

	/* Verify advertised length is smaller than remaining buffer length */
	if (adv_len > len) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*parsed = grabbed;
	
	*content_len = adv_len;

	ret = 0;

out:
	return ret;
}

typedef enum {
	ASN1_TYPE_INTEGER         = 0x02,
	ASN1_TYPE_BIT_STRING      = 0x03,
	ASN1_TYPE_OCTET_STRING    = 0x04,
	ASN1_TYPE_NULL            = 0x05,
	ASN1_TYPE_OID             = 0x06,
	ASN1_TYPE_SEQUENCE        = 0x10,
	ASN1_TYPE_SET             = 0x11,
	ASN1_TYPE_PrintableString = 0x13,
	ASN1_TYPE_T61String       = 0x14,
	ASN1_TYPE_IA5String       = 0x16,
	ASN1_TYPE_UTCTime         = 0x17,
	ASN1_TYPE_GeneralizedTime = 0x18,
} asn1_type;
/*
 * Implements a function for parsing OID as described in section 8.19
 * of X.690. On success, the function returns 0 and set 'parsed'
 * parameters to the amount of bytes on which the OID is encoded
 * (header and content bytes).
 */

static int parse_OID(const u8 *buf, u16 len, u16 *parsed)
{
	u16 data_len = 0;
	u16 hdr_len = 0;
	u16 remain = 0;
	u16 num;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_OID,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	len -= hdr_len;
	buf += hdr_len;
	if (data_len < 1) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}
	

	remain = data_len;
	num = 0;

	
	while (remain) {
		u32 arc_val = 0;
		u16 rbytes = 0;

		/*
		 * RFC 5280 has "Implementations MUST be able to handle
		 * OIDs with up to 20 elements (inclusive)".
		 */
		if (num > 20) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		ret = _parse_arc(buf, remain, &arc_val, &rbytes);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		

		num += 1;

		buf += rbytes;
		remain -= rbytes;
	}

	/*
	 * Let's check the OID had at least the first initial
	 * subidentifier (the one derived from the two first
	 * components) as described in section 8.19 of X.690.
	 */
	if (num < 1) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*parsed = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 * Implements a function for parsing integers as described in section 8.3
 * of X.690. As integers may be used in a context specific way, we allow
 * passing the expected class and type values which are to be found.
 */

static int parse_integer(const u8 *buf, u16 len,
			 tag_class exp_class, u32 exp_type,
			 u16 *eaten)
{
	u16 hdr_len = 0;
	u16 data_len = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = parse_id_len(buf, len, exp_class, exp_type, &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	buf += hdr_len;

	/*
	 * Regarding integer encoding, 8.3.1 of X.690 has "The contents octets
	 * shall consist of one or more octets".
	 */
	if (data_len == 0) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/*
	 * On integer encoding, 8.3.2 of x.690 has "If the contents octets of
	 * an integer value encoding consist of more than one octet, then the
	 * bits of the first octet and bit 8 of the second octet:
	 *
	 *  a) shall not all be ones; and
	 *  b) shall not all be zero.
	 */
	if (data_len > 1) {
		if ((buf[0] == 0) && ((buf[1] & 0x80) == 0)) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		if ((buf[0] == 0xff) && (buf[1] & 0x80)) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 * This function verify given buffer contains a valid UTF-8 string
 * -1 is returned on error, 0 on success.
 */

static int check_utf8_string(const u8 *buf, u16 len)
{
	int ret;
	u8 b0;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	
	while (len) {
		b0 = buf[0];

		/*
		 * CP encoded on a single byte, coding from 1 to 7 bits,
		 * U+000 to U+07FF.
		 */

		if (b0 <= 0x7f) {                   /* 0x00 to 0x7f */
			len -= 1;
			buf += 1;
			continue;
		}

		/*
		 * CP encoded on 2 bytes, coding from 8 to 11 bits,
		 * U+0080 to U+07FF
		 */

		if ((b0 >= 0xc2) && (b0 <= 0xdf)) { /* 0xc2 to 0xdf */
			if (len < 2) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			
			if ((buf[1] & 0xc0) != 0x80) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			len -= 2;
			buf += 2;
			continue;
		}

		/*
		 * CP encoded on 3 bytes, coding 12 to 16 bits,
		 * U+0800 to U+FFFF.
		 */

		if ((b0 >= 0xe0) && (b0 <= 0xef)) { /* 0xe0 to 0xef */
			if (len < 3) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			
			if (((buf[1] & 0xc0) != 0x80) ||
			    ((buf[2] & 0xc0) != 0x80)) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			/*
			 * 1rst byte is 0xe0 => 2nd byte in [0xa0, 0xbf]
			 * 1rst byte is 0xed => 2nd byte in [0x80, 0x9f]
			 */
			if ((b0 == 0xe0) &&
			    ((buf[1] < 0xa0) || (buf[1] > 0xbf))) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			} else if ((b0 == 0xed) &&
				   ((buf[1] < 0x80) || (buf[1] > 0x9f))) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			len -= 3;
			buf += 3;
			continue;
		}

		/*
		 * CP encoded on 4 bytes, coding 17 to 21 bits,
		 * U+10000 to U+10FFFF.
		 */

		if ((b0 >= 0xf0) && (b0 <= 0xf4)) { /* 0xf0 to 0xf4 */
			if (len < 4) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			/*
			 * 1rst byte is 0xe0 => 2nd byte in [0xa0, 0xbf]
			 * 1rst byte is 0xed => 2nd byte in [0x80, 0x9f]
			 */
			
			if ((b0 == 0xf0) &&
			    ((buf[1] < 0x90) || (buf[1] > 0xbf))) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			} else if ((b0 == 0xf4) &&
				   ((buf[1] < 0x80) || (buf[1] > 0x8f))) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			if (((buf[1] & 0xc0) != 0x80) ||
			    ((buf[2] & 0xc0) != 0x80) ||
			    ((buf[3] & 0xc0) != 0x80)) {
				ret = -__LINE__;
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			len -= 4;
			buf += 4;
			continue;
		}

		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * VisibleString == ISO646String == ASCII
 */

static int check_visible_string(const u8 *buf, u16 len)
{
	int ret;
	u16 rbytes = 0;
	u8 c;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	
	while (rbytes < len) {
		c = buf[rbytes];

		if ((c >= 'a' && c <= 'z') ||
		    (c >= 'A' && c <= 'Z') ||
		    (c >= '0' && c <= '9')) {
			rbytes += 1;
			continue;
		}

		switch (c) {
		case 39: /* ' */
		case '=':
		case '(':
		case ')':
		case '+':
		case ',':
		case '-':
		case '.':
		case '/':
		case ':':
		case '?':
		case ' ':
		case '!':
		case '"':
		case '#':
		case '$':
		case '%':
		case '&':
		case '*':
		case ';':
		case '<':
		case '>':
		case '[':
		case '\\':
		case ']':
		case '^':
		case '_':
		case '`':
		case '{':
		case '|':
		case '}':
		case '~':
			rbytes += 1;
			continue;
		default:
			break;
		}

		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = 0;

out:
	return ret;
}


static int check_bmp_string(const u8 ATTRIBUTE_UNUSED *buf,
			    u16 ATTRIBUTE_UNUSED len)
{
	/* Support is OPTIONAL */
	return -__LINE__;
}


static int check_ia5_string(const u8 *buf, u16 len)
{
	int ret;
	u16 i;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	
	for (i = 0; i < len; i++) {
		if (buf[i] > 0x7f) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}
	}

	ret = 0;

out:
	return ret;
}

#define STR_TYPE_UTF8_STRING      12
#define STR_TYPE_PRINTABLE_STRING 19
#define STR_TYPE_TELETEX_STRING   20
#define STR_TYPE_IA5_STRING       22
#define STR_TYPE_VISIBLE_STRING   26
#define STR_TYPE_UNIVERSAL_STRING 28
#define STR_TYPE_BMP_STRING       30
/*
 * As pointed by RFC 4519 and described by RFC 4517, IA5String
 * is defined in ABNF form as *(%x00-7F).
 */

static int parse_ia5_string(const u8 *buf, u16 len, u16 lb, u16 ub)
{
	int ret = -__LINE__;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	if (len < 2) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	str_type = buf[0];
	if (str_type != STR_TYPE_IA5_STRING) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	len -= 2;
	if (buf[1] != len) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = check_ia5_string(buf, len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/* CPSuri ::= IA5String */

static int parse_CPSuri(const u8 *buf, u16 len, u16 *eaten)
{
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = parse_ia5_string(buf, len, 1, 65534);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*eaten = len;

	ret = 0;

out:
	return ret;
}


/*
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 */

static int parse_DisplayText(const u8 *buf, u16 len, u16 *eaten)
{
	u16 hdr_len = 0, data_len = 0;
	u8 str_type;
	int ret = -1;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	str_type = buf[0];

	switch (str_type) {
	case STR_TYPE_UTF8_STRING:    /* UTF8String */
	case STR_TYPE_IA5_STRING:     /* IA5String */
	case STR_TYPE_VISIBLE_STRING: /* VisibileString */
	case STR_TYPE_BMP_STRING:     /* BMPString */
		ret = parse_id_len(buf, len, CLASS_UNIVERSAL, str_type,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		buf += hdr_len;

		switch (str_type) {
		case STR_TYPE_UTF8_STRING:
			ret = check_utf8_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}
			break;
		case STR_TYPE_IA5_STRING:
			ret = check_ia5_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}
			break;
		case STR_TYPE_VISIBLE_STRING:
			ret = check_visible_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}
			break;
		case STR_TYPE_BMP_STRING:
			ret = check_bmp_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}
			break;
		default:
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
			break;
		}

		*eaten = hdr_len + data_len;

		break;
	default:
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
		break;
	}

out:
	return ret;
}


/*
 *     NoticeReference ::= SEQUENCE {
 *          organization     DisplayText,
 *          noticeNumbers    SEQUENCE OF INTEGER }
 *
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 *
 */

static int parse_NoticeReference(const u8 *buf, u16 len, u16 *eaten)
{
	u16 remain, parsed = 0, saved_len = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain = len;

	/* NoticeReference is a sequence */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	saved_len = hdr_len + data_len;
	remain = data_len;
	buf += hdr_len;

	/*
	 * First element of the sequence is the organization (of type
	 * DisplayText)
	 */
	ret = parse_DisplayText(buf, remain, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain -= parsed;
	buf += parsed;

	/*
	 * Second element is the noticeNumbers, i.e. a sequence of
	 * integers.
	 */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain -= hdr_len;
	buf += hdr_len;

	/* Advertised data in the sequence must exactly match what remains */
	if (remain != data_len) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* The sequence contains integers */
	
	while (remain) {
		/* Verify the integer is encoded as it should */
		ret = parse_integer(buf, remain,
				    CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				    &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		remain -= parsed;
		buf += parsed;
	}

	*eaten = saved_len;

	ret = 0;

out:
	return ret;
}

/*
 *     UserNotice ::= SEQUENCE {
 *          noticeRef        NoticeReference OPTIONAL,
 *          explicitText     DisplayText OPTIONAL }
 *
 *     NoticeReference ::= SEQUENCE {
 *          organization     DisplayText,
 *          noticeNumbers    SEQUENCE OF INTEGER }
 *
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 *
 */

static int parse_UserNotice(const u8 *buf, u16 len, u16 *eaten)
{
	u16 hdr_len = 0, data_len = 0, remain = 0, parsed = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain = len;

	/* USerNotice is a sequence */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain -= hdr_len;
	buf += hdr_len;

	/* Having an empty sequence is considered invalid */
	if (!data_len) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* First element (if present) is a noticeRef of type NoticeReference */
	ret = parse_NoticeReference(buf, remain, &parsed);
	if (!ret) {
		remain -= parsed;
		buf += parsed;
	}

	/* Second element (if present) is an explicitText of type DisplayText */
	ret = parse_DisplayText(buf, remain, &parsed);
	if (!ret) {
		remain -= parsed;
		buf += parsed;
	}

	if (remain) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}


/*
 *   PolicyQualifierInfo ::= SEQUENCE {
 *        policyQualifierId  PolicyQualifierId,
 *        qualifier          ANY DEFINED BY policyQualifierId }
 *
 *   -- policyQualifierIds for Internet policy qualifiers
 *
 *   id-qt          OBJECT IDENTIFIER ::=  { id-pkix 2 }
 *   id-qt-cps      OBJECT IDENTIFIER ::=  { id-qt 1 }
 *   id-qt-unotice  OBJECT IDENTIFIER ::=  { id-qt 2 }
 *
 *   PolicyQualifierId ::= OBJECT IDENTIFIER ( id-qt-cps | id-qt-unotice )
 */

static int parse_policyQualifierInfo(const u8 *buf, u16 len, u16 *eaten)
{
	u16 hdr_len = 0, data_len = 0, oid_len = 0, remain = 0;
	u8 id_qt_cps_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05,
			       0x07, 0x02, 0x01 };
	u8 id_qt_unotice_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05,
				   0x07, 0x02, 0x02 };
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* It's a sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;

	/*
	 * First element of the sequence (policyQualifierId) is an OID
	 * which can either take a value of id-qt-cps or id-qt-unotice.
	 */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	if ((oid_len == sizeof(id_qt_cps_oid)) &&
	    !bufs_differ(buf, id_qt_cps_oid, oid_len)) { /* id-qt-cps */
		u16 cpsuri_len = 0;

		buf += oid_len;
		remain -= oid_len;

		ret = parse_CPSuri(buf, remain, &cpsuri_len);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		remain -= cpsuri_len;
		buf += cpsuri_len;

	} else if ((oid_len == sizeof(id_qt_unotice_oid)) &&
	    !bufs_differ(buf, id_qt_unotice_oid, oid_len)) { /* id-qt-unotice */
		u16 cpsunotice_len = 0;

		buf += oid_len;
		remain -= oid_len;

		ret = parse_UserNotice(buf, remain, &cpsunotice_len);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		remain -= cpsunotice_len;
		buf += cpsunotice_len;

	} else {                                        /* unsupported! */
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}


/*
 *   PolicyInformation ::= SEQUENCE {
 *        policyIdentifier   CertPolicyId,
 *        policyQualifiers   SEQUENCE SIZE (1..MAX) OF
 *                                PolicyQualifierInfo OPTIONAL }
 */

static int parse_PolicyInformation(const u8 *buf, u16 len, u16 *eaten)
{
	u16 hdr_len = 0, data_len = 0, oid_len = 0, saved_pi_len, remain;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	saved_pi_len = hdr_len + data_len;

	remain = data_len;
	buf += hdr_len;

	/* policyIdentifier is a CertPolicyId, i.e. an OID */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	remain -= oid_len;
	buf += oid_len;

	/* policyQualifiers is optional */
	if (remain) {
		/* policyQualifiers is a sequence */
		ret = parse_id_len(buf, remain,
				   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		remain -= hdr_len;
		buf += hdr_len;

		/* Nothing should remain after policyQualifiers */
		if (remain != data_len) {
			ret = -__LINE__;
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		/*
		 * Let's parse each PolicyQualifierInfo in the
		 * policyQualifiers sequence
		 */
		
		while (remain) {
			u16 pqi_len = 0;

			ret = parse_policyQualifierInfo(buf, remain, &pqi_len);
			if (ret) {
				ERROR_TRACE_APPEND(__LINE__);
				goto out;
			}

			remain -= pqi_len;
			buf += pqi_len;
		}
	}

	*eaten = saved_pi_len;

	/*
	 * FIXME! At that point, we should verify we know the OID
	 * (policyIdentifier) and the associated optional
	 * content is indeed valid.
	 */

	ret = 0;

out:
	return ret;
}


typedef struct {
	int empty_subject;
	int san_empty;
	int san_critical;
	int ca_true;
	int bc_critical;
	int has_ski;
	int has_keyUsage;
	int keyCertSign_set;
	int cRLSign_set;
	int pathLenConstraint_set;
	int has_name_constraints;
	int has_crldp;
	int one_crldp_has_all_reasons;
	int aki_has_keyIdentifier;
	int self_signed;
} cert_parsing_ctx;
/* 4.2.1.4. Certificate Policies
 *
 * id-ce-certificatePolicies OBJECT IDENTIFIER ::=  { id-ce 32 }
 *
 *   anyPolicy OBJECT IDENTIFIER ::= { id-ce-certificatePolicies 0 }
 *
 *   certificatePolicies ::= SEQUENCE SIZE (1..MAX) OF PolicyInformation
 *
 *   PolicyInformation ::= SEQUENCE {
 *        policyIdentifier   CertPolicyId,
 *        policyQualifiers   SEQUENCE SIZE (1..MAX) OF
 *                                PolicyQualifierInfo OPTIONAL }
 *
 *   CertPolicyId ::= OBJECT IDENTIFIER
 *
 *   PolicyQualifierInfo ::= SEQUENCE {
 *        policyQualifierId  PolicyQualifierId,
 *        qualifier          ANY DEFINED BY policyQualifierId }
 *
 *   -- policyQualifierIds for Internet policy qualifiers
 *
 *   id-qt          OBJECT IDENTIFIER ::=  { id-pkix 2 }
 *   id-qt-cps      OBJECT IDENTIFIER ::=  { id-qt 1 }
 *   id-qt-unotice  OBJECT IDENTIFIER ::=  { id-qt 2 }
 *
 *   PolicyQualifierId ::= OBJECT IDENTIFIER ( id-qt-cps | id-qt-unotice )
 *
 *   Qualifier ::= CHOICE {
 *        cPSuri           CPSuri,
 *        userNotice       UserNotice }
 *
 *   CPSuri ::= IA5String
 *
 *   UserNotice ::= SEQUENCE {
 *        noticeRef        NoticeReference OPTIONAL,
 *        explicitText     DisplayText OPTIONAL }
 *
 *   NoticeReference ::= SEQUENCE {
 *        organization     DisplayText,
 *        noticeNumbers    SEQUENCE OF INTEGER }
 *
 *   DisplayText ::= CHOICE {
 *        ia5String        IA5String      (SIZE (1..200)),
 *        visibleString    VisibleString  (SIZE (1..200)),
 *        bmpString        BMPString      (SIZE (1..200)),
 *        utf8String       UTF8String     (SIZE (1..200)) }
 *
 */

static int parse_ext_certPolicies(const u8 *buf, u16 len,
				  int ATTRIBUTE_UNUSED critical,
				  cert_parsing_ctx ATTRIBUTE_UNUSED *ctx)
{
	u16 remain = 0, data_len = 0, hdr_len = 0, eaten = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/*
	 * FIXME!
	 *
	 * This one will be a pain to deal with if we decide
	 * to support the full version, i.e. non empty sequence
	 * for policyQualifiersOID. RFC 5280 has:
	 *
	 *  To promote interoperability, this profile RECOMMENDS that policy
	 *  information terms consist of only an OID.	 Where an OID alone is
	 *  insufficient, this profile strongly recommends that the use of
	 *  qualifiers be limited to those identified in this section.  When
	 *  qualifiers are used with the special policy anyPolicy, they MUST be
	 *  limited to the qualifiers identified in this section.  Only those
	 *  qualifiers returned as a result of path validation are considered.
	 *
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -__LINE__;
		ERROR_TRACE_APPEND(__LINE__);
		goto out;
	}

	/* Let's now check each individual PolicyInformation sequence */
	
	while (remain) {
		ret = parse_PolicyInformation(buf, remain, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(__LINE__);
			goto out;
		}

		remain -= eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}