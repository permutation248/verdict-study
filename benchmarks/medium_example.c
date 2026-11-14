/*
 * Medium example: 5 functions, three-level call graph with a loop.
 *
 * Call graph:
 *
 *     main_entry
 *          │
 *          ├──► compute_average
 *          │        │
 *          │        └──► sum_array   (has a loop)
 *          │
 *          └──► validate_buffer       (has a loop, calls is_nonzero)
 *                   │
 *                   └──► is_nonzero   (leaf)
 *
 * Intent:
 *   - 3-level call graph with a diamond (compute_average and validate_buffer
 *     are both reachable from main_entry).
 *   - sum_array and validate_buffer each have a for loop → loop_contract VOs
 *     (invariant + variant).
 *   - Several call-site precondition VOs per caller.
 *   - RTE VOs: pointer dereference, array indexing, integer overflow, division
 *     by zero.
 *   - Enough surface area to see multiple reactivation cycles, vo_failed
 *     wake reasons, and cross-agent proof demands.
 */

int is_nonzero(int x)
{
    return x != 0 ? 1 : 0;
}

int validate_buffer(const int *buf, int len)
{
    if (buf == 0) {
        return 0;
    }
    if (len <= 0) {
        return 0;
    }
    //loop_name validate_loop
    for (int i = 0; i < len; i++) {
        if (!is_nonzero(buf[i])) {
            return 0;
        }
    }
    return 1;
}

int sum_array(const int *buf, int len)
{
    int acc = 0;
    //loop_name sum_loop
    for (int i = 0; i < len; i++) {
        acc += buf[i];
    }
    return acc;
}

int compute_average(const int *buf, int len)
{
    if (len <= 0) {
        return 0;
    }
    int total = sum_array(buf, len);
    return total / len;
}

int main_entry(const int *buf, int len)
{
    if (!validate_buffer(buf, len)) {
        return -1;
    }
    return compute_average(buf, len);
}
