#!/usr/bin/env bash
# Benchmark registry: name -> source file (-f) and header (-H).
# Paths are relative to the repository root. Sourced by run scripts.
#
# Three groups, uniform shape benchmarks/<group>/<module>/<module>.c:
#   x509/       ANSSI X.509 parser: submodule-1, submodule-2, full-module
#   embedded/   a module extracted from a production embedded/IoT stack
#   util-libs/  small open-source C utility libraries (SpecSyn eval corpus)

bench_source() {
  case "$1" in
    # x509 (ANSSI parser): two submodules + the whole module.
    submodule-1) echo "benchmarks/x509/submodule-1/x509-submodule-1.c" ;;
    submodule-2) echo "benchmarks/x509/submodule-2/x509-submodule-2.c" ;;
    full-module) echo "benchmarks/x509/full-module/x509-full-module.c" ;;
    # embedded: production-stack module (single self-contained file).
    contiki-aes-ccm) echo "benchmarks/embedded/contiki-aes-ccm/contiki_aes_ccm.c" ;;
    # util-libs: utility-library corpus (single self-contained file each).
    zpl-math)           echo "benchmarks/util-libs/zpl-math/zpl_math.c" ;;
    hypatia)            echo "benchmarks/util-libs/hypatia/hypatia.c" ;;
    char-utils)         echo "benchmarks/util-libs/char-utils/char_utils.c" ;;
    maratis-math)       echo "benchmarks/util-libs/maratis-math/maratis_math.c" ;;
    sokol-shape)        echo "benchmarks/util-libs/sokol-shape/sokol_shape.c" ;;
    ccan-time)          echo "benchmarks/util-libs/ccan-time/ccan_time.c" ;;
    jc-voronoi)         echo "benchmarks/util-libs/jc-voronoi/jc_voronoi.c" ;;
    student-enrollment) echo "benchmarks/util-libs/student-enrollment/student_enrollment.c" ;;
    cgl)                echo "benchmarks/util-libs/cgl/cgl.c" ;;
    punity)             echo "benchmarks/util-libs/punity/punity.c" ;;
    stb-divide)         echo "benchmarks/util-libs/stb-divide/stb_divide.c" ;;
    *) return 1 ;;
  esac
}

# Header file for the benchmark, or empty if it is a single self-contained file.
bench_header() {
  case "$1" in
    submodule-1) echo "benchmarks/x509/submodule-1/x509-parser.h" ;;
    submodule-2) echo "benchmarks/x509/submodule-2/x509-parser.h" ;;
    full-module) echo "benchmarks/x509/full-module/x509-parser.h" ;;
    contiki-aes-ccm) echo "" ;;
    zpl-math|hypatia|char-utils|maratis-math|sokol-shape|ccan-time|jc-voronoi|student-enrollment|cgl|punity|stb-divide) echo "" ;;
    *) return 1 ;;
  esac
}

# WP model override: util-libs use Typed+var+int+real (float-heavy);
# x509 and embedded use default (sound Typed).
bench_wp_model() {
  case "$1" in
    zpl-math|hypatia|char-utils|maratis-math|sokol-shape|ccan-time|jc-voronoi|student-enrollment|cgl|punity|stb-divide)
      echo "Typed+var+int+real" ;;
    *) echo "" ;;
  esac
}

# Source-file stem verdict uses to name the run dir: logs/<stem>_<timestamp>/.
bench_stem() {
  local src; src="$(bench_source "$1")" || return 1
  basename "$src" .c
}
