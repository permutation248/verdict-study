#include <stdbool.h>
#include <stdlib.h>


typedef long time_t;

struct timespec
{
    time_t tv_sec;  // Seconds - >= 0
    long   tv_nsec; // Nanoseconds - [0, 999999999]
};

struct timerel { struct timespec ts; };
struct timeabs { struct timespec ts; };
struct timemono { struct timespec ts; };

typedef unsigned long long uint64_t;

#define TIME_CHECK(t) (t)
#define TIMEREL_CHECK(t) (t)
#define TIMEABS_CHECK(t) (t)
#define TIMEMONO_CHECK(t) (t)



bool time_greater_(struct timespec a, struct timespec b)
{
    if (TIME_CHECK(a).tv_sec > TIME_CHECK(b).tv_sec)
        return true;
    else if (a.tv_sec < b.tv_sec)
         return false;
    return a.tv_nsec > b.tv_nsec;
}



bool time_after(struct timeabs a, struct timeabs b)
{
    return time_greater_(a.ts, b.ts);
}



bool time_greater(struct timerel a, struct timerel b)
{
    return time_greater_(a.ts, b.ts);
}



bool time_less_(struct timespec a, struct timespec b)
{
    if (TIME_CHECK(a).tv_sec < TIME_CHECK(b).tv_sec)
        return true;
    else if (a.tv_sec > b.tv_sec)
         return false;
    return a.tv_nsec < b.tv_nsec;
}



bool time_before(struct timeabs a, struct timeabs b)
{
    return time_less_(a.ts, b.ts);
}



bool time_less(struct timerel a, struct timerel b)
{
    return time_less_(a.ts, b.ts);
}



bool timeabs_eq(struct timeabs a, struct timeabs b)
{
    return TIMEABS_CHECK(a).ts.tv_sec == TIMEABS_CHECK(b).ts.tv_sec
        && a.ts.tv_nsec == b.ts.tv_nsec;
}



bool timemono_eq(struct timemono a, struct timemono b)
{
    return TIMEMONO_CHECK(a).ts.tv_sec == TIMEMONO_CHECK(b).ts.tv_sec
        && a.ts.tv_nsec == b.ts.tv_nsec;
}



bool timerel_eq(struct timerel a, struct timerel b)
{
    return TIMEREL_CHECK(a).ts.tv_sec == TIMEREL_CHECK(b).ts.tv_sec
        && a.ts.tv_nsec == b.ts.tv_nsec;
}



struct timespec time_sub_(struct timespec recent, struct timespec old)
{
    struct timespec x, y, diff;

    // 选大的作为 x，保证 x >= y
    if (recent.tv_sec < old.tv_sec ||
        (recent.tv_sec == old.tv_sec && recent.tv_nsec < old.tv_nsec)) {
        x = old;
        y = recent;
    } else {
        x = recent;
        y = old;
    }

    x = TIME_CHECK(x);
    y = TIME_CHECK(y);

    diff.tv_sec = x.tv_sec - y.tv_sec;
    if (y.tv_nsec > x.tv_nsec) {
        diff.tv_sec--;
        diff.tv_nsec = 1000000000 + x.tv_nsec - y.tv_nsec;
    } else {
        diff.tv_nsec = x.tv_nsec - y.tv_nsec;
    }

    return TIME_CHECK(diff);
}



struct timerel time_sub(struct timerel a, struct timerel b)
{
    struct timespec x, y;
    struct timerel t;

    // pick the larger one as x
    if (a.ts.tv_sec < b.ts.tv_sec ||
        (a.ts.tv_sec == b.ts.tv_sec && a.ts.tv_nsec < b.ts.tv_nsec)) {
        x = b.ts;
        y = a.ts;
    } else {
        x = a.ts;
        y = b.ts;
    }

    t.ts = time_sub_(x, y);
    return t;
}



struct timerel time_between(struct timeabs recent, struct timeabs old)
{
    struct timerel t;
    t.ts = time_sub_(recent.ts, old.ts);
    return t;
}



struct timerel timemono_between(struct timemono recent, struct timemono old)
{
    struct timerel t;
    t.ts = time_sub_(recent.ts, old.ts);
    return t;
}



struct timeabs timeabs_sub(struct timeabs abs, struct timerel rel)
{
    struct timeabs t;
    t.ts = time_sub_(abs.ts, rel.ts);
    return t;
}



struct timespec time_add_(struct timespec a, struct timespec b)
{
    struct timespec sum;
    sum.tv_sec = TIME_CHECK(a).tv_sec + TIME_CHECK(b).tv_sec;
    sum.tv_nsec = a.tv_nsec + b.tv_nsec;
    if (sum.tv_nsec >= 1000000000) {
        sum.tv_sec++;
        sum.tv_nsec -= 1000000000;
    }
    return TIME_CHECK(sum);
}



struct timeabs timeabs_add(struct timeabs a, struct timerel b)
{
    struct timeabs t;
    t.ts = time_add_(a.ts, b.ts);
    return t;
}



struct timemono timemono_add(struct timemono a, struct timerel b)
{
    struct timemono t;
    t.ts = time_add_(a.ts, b.ts);
    return t;
}



struct timerel timerel_add(struct timerel a, struct timerel b)
{
    struct timerel t;
    t.ts = time_add_(a.ts, b.ts);
    return t;
}



struct timerel time_divide(struct timerel t, unsigned long div)
{
    struct timerel res;
    uint64_t rem, ns;

    res.ts.tv_sec = TIMEREL_CHECK(t).ts.tv_sec / div;
    rem = t.ts.tv_sec % div;

    if (rem & ~(((uint64_t)1 << 30) - 1)) {
        double nsec = rem * 1000000000.0 + t.ts.tv_nsec;
        res.ts.tv_nsec = nsec / div;
    } else {
        ns = rem * 1000000000 + t.ts.tv_nsec;
        res.ts.tv_nsec = ns / div;
    }
    return TIMEREL_CHECK(res);
}



struct timerel time_multiply(struct timerel t, unsigned long mult)
{
    struct timerel res;

    if (mult & ~((1UL << 30) - 1)) {
        double nsec = (double)t.ts.tv_nsec * mult;
        res.ts.tv_sec = nsec / 1000000000.0;
        res.ts.tv_nsec = nsec - (res.ts.tv_sec * 1000000000.0);
    } else {
        uint64_t nsec = t.ts.tv_nsec * mult;
        res.ts.tv_nsec = nsec % 1000000000;
        res.ts.tv_sec = nsec / 1000000000;
    }
    res.ts.tv_sec += TIMEREL_CHECK(t).ts.tv_sec * mult;
    return TIMEREL_CHECK(res);
}



uint64_t time_to_sec(struct timerel t)
{
    return t.ts.tv_sec;
}


/* time_to_msec: (sec, nsec) -> milliseconds
   sec_part = sec * 1000
   nsec_part = nsec / 1_000_000  (range 0..999)
   To guarantee sec_part + nsec_part <= UINT64_MAX we require:
     sec <= floor((UINT64_MAX - 999) / 1000) = 18446744073709550
*/

uint64_t time_to_msec(struct timerel t)
{
    uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000;
    uint64_t nsec_part = (uint64_t)(t.ts.tv_nsec / (long)1000000);

    return sec_part + nsec_part;
}


/* time_to_usec: (sec, nsec) -> microseconds
   sec_part = sec * 1_000_000
   nsec_part = nsec / 1000  (range 0..999999)
   require sec <= floor((UINT64_MAX - 999999) / 1000000) = 18446744073708
*/

uint64_t time_to_usec(struct timerel t)
{
    uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000000;
    uint64_t nsec_part = (uint64_t)(t.ts.tv_nsec / (long)1000);

    return sec_part + nsec_part;
}


/* time_to_nsec: (sec, nsec) -> nanoseconds
   sec_part = sec * 1_000_000_000
   nsec_part = nsec  (range 0..999999999)
   require sec <= floor((UINT64_MAX - 999999999) / 1000000000) = 18446744
*/

uint64_t time_to_nsec(struct timerel t)
{
    uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000000000;
    uint64_t nsec_part = (uint64_t)t.ts.tv_nsec;

    return sec_part + nsec_part;
}



struct timerel time_from_sec(uint64_t sec)
{
    struct timerel t;
    t.ts.tv_nsec = (long)0;
    t.ts.tv_sec = (time_t)sec;
    return t;
}



struct timerel time_from_msec(uint64_t msec)
{
    struct timerel t;
    t.ts.tv_nsec = (msec % 1000) * 1000000;
    t.ts.tv_sec = msec / 1000;
    return TIMEREL_CHECK(t);
}



struct timerel time_from_usec(uint64_t usec)
{
    struct timerel t;
    t.ts.tv_nsec = (usec % 1000000) * 1000;
    t.ts.tv_sec = usec / 1000000;
    return TIMEREL_CHECK(t);
}



struct timerel time_from_nsec(uint64_t nsec)
{
    struct timerel t;
    t.ts.tv_nsec = nsec % 1000000000;
    t.ts.tv_sec = nsec / 1000000000;
    return TIMEREL_CHECK(t);
}