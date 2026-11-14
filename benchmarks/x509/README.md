# x509 ASN.1 Parser Benchmark

Source: [MUSIC/x509-parser](https://github.com/music-dsp-collection/x509-parser)
(a standalone, dependency-free x509 certificate/CRL parser in C).

## Files

| File | Functions | Lines | Description |
|------|-----------|-------|-------------|
| `x509-submodule-2_cleaned_26.c` | 20 | 1635 | Certificate policy subsystem (certPolicies extension parsing). Self-contained: no function pointers, no external deps. Used as the primary benchmark. |
| `x509-full-module.c (formerly x509-full-cleaned.c)` | ~270 | 20423 | Full parser (cert + CRL + common utilities). All function pointer dispatch inlined as if-else chains. |
| `x509-parser.h` | — | — | Shared header (type definitions, constants). |

## Function pointer elimination

The original x509-parser uses 8 function-pointer dispatch tables for
algorithm/extension plugin selection. Frama-C WP cannot reason about
indirect calls, so `x509-full-module.c` replaces all 10 indirect call
sites with equivalent if-else chains.

Two replacement styles were used depending on the dispatch pattern:

### Style A: compare the function pointer field

Used when the dispatch table entry is looked up first, then a field is
called. The replacement compares the function pointer field against
every concrete function that can appear in the table.

Original:
```c
ret = talg->parse_algoid_sig_params(&ctx->sig_alg_params, &ctx->hash_alg, cert, off, param_len);
```

Cleaned:
```c
if (talg->parse_algoid_sig_params == &parse_algoid_sig_params_ecdsa_with) {
    ret = parse_algoid_sig_params_ecdsa_with(&ctx->sig_alg_params, &ctx->hash_alg, cert, off, param_len);
} else if (talg->parse_algoid_sig_params == &parse_algoid_sig_params_sm2) {
    ret = parse_algoid_sig_params_sm2(&ctx->sig_alg_params, &ctx->hash_alg, cert, off, param_len);
} else if (...) { ... }
```

Applied to: `parse_algoid_sig_params` (16 branches), `parse_sig` (32),
`parse_algoid_pubkey_params` (14), `parse_pubkey` (12),
`parse_ext_params` (16).

### Style B: compare the dispatch table entry pointer

Used when the caller iterates over a known-OID table and resolves an
entry pointer before calling. The replacement compares the entry pointer
against the table's static array elements.

Original:
```c
ret = cur->parse_rdn_val(buf, data_len);
```

Cleaned:
```c
if (cur == &known_dn_oids[0]) {
    ret = parse_rdn_val_cn(buf, data_len);
} else if (cur == &known_dn_oids[1]) {
    ret = parse_rdn_val_x520name(buf, data_len);
} else if (...) { ... }
```

Applied to: `parse_rdn_val` (23 table entries),
`parse_crl_entry_ext_params` (3), `parse_crl_ext_params` (12).

### Correctness argument

Both styles are semantically equivalent to the original indirect call:
the if-else chain exhaustively enumerates every function pointer value
that appears in the dispatch table's static initializers. Since the
tables are `const` and only initialized at compile time, no runtime
value can fall outside the enumerated set. The original source's ACSL
annotations (`\valid_function` and `\in {...}` assertions) confirm
the same closed-world assumption.

### Summary

| Function pointer | Original call sites | Replacement style | Branches | Indirect calls remaining |
|---|---|---|---|---|
| `parse_algoid_sig_params` | 2 (cert + CRL) | A | 16 | 0 |
| `parse_sig` | 2 (cert + CRL) | A | 32 | 0 |
| `parse_algoid_pubkey_params` | 1 (cert) | A | 14 | 0 |
| `parse_pubkey` | 1 (cert) | A | 12 | 0 |
| `parse_ext_params` | 1 (cert) | A | 16 | 0 |
| `parse_rdn_val` | 1 (common) | B | 23 | 0 |
| `parse_crl_entry_ext_params` | 1 (CRL) | B | 3 | 0 |
| `parse_crl_ext_params` | 1 (CRL) | B | 12 | 0 |
