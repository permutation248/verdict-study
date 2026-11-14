int stb__div(int v1, int v2);
int stb__mod(int v1, int v2);
int stb_div_trunc(int v1, int v2);
int stb_div_eucl(int v1, int v2);
int stb_mod_trunc(int v1, int v2);
int stb_mod_floor(int v1, int v2);
int stb_mod_eucl(int v1, int v2);

/*@
requires v2 != 0;
requires v1 != (-2147483647 - 1) || v2 != -1;
assigns \nothing;
ensures (v1 >= 0 && v2 > 0) ==> (\result >= 0);
ensures (v1 <= 0 && v2 > 0) ==> (\result <= 0);
ensures (v1 >= 0 && v2 < 0) ==> (\result <= 0);
ensures (v1 <= 0 && v2 < 0) ==> (\result >= 0);
ensures v2 > 0 ==> \result * v2 <= v1;
ensures v2 < 0 ==> \result * v2 >= v1;
ensures (v1 >= 0 && v2 > 0) ==> (\result <= v1);
ensures (v1 >= 0 && v2 >= 2) ==> (\result <= v1 / 2);
ensures (v1 <= 0 && v2 > 0) ==> (\result >= v1);
ensures (v1 <= 0 && v2 <= -2) ==> (\result <= (-v1) / 2);
*/
int stb__div(int v1, int v2)
{
  int __retres;
  /*@ assert rte_1: division_by_zero: v2 ≢ 0; */
  /*@ assert rte_2: signed_overflow: v1 / v2 ≤ 2147483647; */
  int q = v1 / v2;
  /*@ assert rte_3: division_by_zero: v2 ≢ 0; */
  int r = v1 % v2;
  if (r > 0) {
    if (v2 < 0) {
      /*@ assert rte_4: signed_overflow: -2147483648 ≤ q - 1; */
      __retres = q - 1;
      goto return_label;
    }
    else goto _LAND;
  }
  else {
    _LAND: ;
    if (r < 0) 
      if (v2 > 0) {
        /*@ assert rte_5: signed_overflow: -2147483648 ≤ q - 1; */
        __retres = q - 1;
        goto return_label;
      }
      else {
        __retres = q;
        goto return_label;
      }
    else {
      __retres = q;
      goto return_label;
    }
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
assigns \nothing;
ensures (v2 > 0) ==> (0 <= \result < v2);
ensures (v2 < 0) ==> (v2 < \result <= 0);
*/
int stb__mod(int v1, int v2)
{
  int __retres;
  /*@ assert rte_6: division_by_zero: v2 ≢ 0; */
  int r = v1 % v2;
  if (r > 0) {
    if (v2 < 0) {
      /*@ assert rte_7: signed_overflow: -2147483648 ≤ r + v2; */
      /*@ assert rte_8: signed_overflow: r + v2 ≤ 2147483647; */
      __retres = r + v2;
      goto return_label;
    }
    else goto _LAND;
  }
  else {
    _LAND: ;
    if (r < 0) 
      if (v2 > 0) {
        /*@ assert rte_9: signed_overflow: -2147483648 ≤ r + v2; */
        /*@ assert rte_10: signed_overflow: r + v2 ≤ 2147483647; */
        __retres = r + v2;
        goto return_label;
      }
      else {
        __retres = r;
        goto return_label;
      }
    else {
      __retres = r;
      goto return_label;
    }
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
requires v1 != -2147483648 || v2 != -1;
assigns \nothing;
ensures (v1 >= 0 && v2 > 0) ==> (\result >= 0);
ensures (v1 <= 0 && v2 > 0) ==> (\result <= 0);
ensures (v1 >= 0 && v2 < 0) ==> (\result <= 0);
ensures (v1 <= 0 && v2 < 0) ==> (\result >= 0);
*/
int stb_div_trunc(int v1, int v2)
{
  int __retres;
  if (v1 >= 0) 
    if (v2 <= 0) {
      int tmp;
      /*@ assert rte_48: signed_overflow: -2147483647 ≤ v1; */
      tmp = stb__div(- v1,v2);
      /*@ assert rte_49: signed_overflow: -2147483647 ≤ tmp; */
      __retres = - tmp;
      goto return_label;
    }
  if (v1 <= 0) 
    if (v2 >= 0) 
      if ((long)v1 != -2147483648) {
        int tmp_0;
        /*@ assert rte_50: signed_overflow: -2147483647 ≤ v2; */
        tmp_0 = stb__div(v1,- v2);
        /*@ assert rte_51: signed_overflow: -2147483647 ≤ tmp_0; */
        __retres = - tmp_0;
        goto return_label;
      }
      else {
        int tmp_1;
        /*@ assert rte_52: signed_overflow: -2147483648 ≤ v1 + v2; */
        /*@ assert rte_53: signed_overflow: v1 + v2 ≤ 2147483647; */
        /*@ assert rte_54: signed_overflow: -2147483647 ≤ v2; */
        tmp_1 = stb__div(v1 + v2,- v2);
        /*@ assert rte_55: signed_overflow: -2147483647 ≤ tmp_1; */
        /*@ assert rte_56: signed_overflow: -2147483648 ≤ (int)(-tmp_1) - 1;
        */
        __retres = - tmp_1 - 1;
        goto return_label;
      }
    else {
      /*@ assert rte_57: division_by_zero: v2 ≢ 0; */
      /*@ assert rte_58: signed_overflow: v1 / v2 ≤ 2147483647; */
      __retres = v1 / v2;
      goto return_label;
    }
  else {
    /*@ assert rte_59: division_by_zero: v2 ≢ 0; */
    /*@ assert rte_60: signed_overflow: v1 / v2 ≤ 2147483647; */
    __retres = v1 / v2;
    goto return_label;
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
requires !(v1 == -2147483648 && v2 == -1);
assigns \nothing;
*/
int stb_div_eucl(int v1, int v2)
{
  int __retres;
  int q;
  int r;
  if (v1 >= 0) 
    if (v2 >= 0) {
      int tmp;
      tmp = stb__div(v1,v2);
      __retres = tmp;
      goto return_label;
    }
    else 
      if ((long)v2 != -2147483648) {
        int tmp_0;
        /*@ assert rte_11: signed_overflow: -2147483647 ≤ v2; */
        tmp_0 = stb__div(v1,- v2);
        /*@ assert rte_12: signed_overflow: -2147483647 ≤ tmp_0; */
        q = - tmp_0;
        /*@ assert rte_13: signed_overflow: -2147483647 ≤ v2; */
        r = stb__mod(v1,- v2);
      }
      else {
        q = 0;
        r = v1;
      }
  else 
    if ((long)v1 != -2147483648) 
      if (v2 >= 0) {
        int tmp_1;
        int tmp_2;
        /*@ assert rte_14: signed_overflow: -2147483647 ≤ v1; */
        tmp_1 = stb__div(- v1,v2);
        /*@ assert rte_15: signed_overflow: -2147483647 ≤ tmp_1; */
        q = - tmp_1;
        /*@ assert rte_16: signed_overflow: -2147483647 ≤ v1; */
        tmp_2 = stb__mod(- v1,v2);
        /*@ assert rte_17: signed_overflow: -2147483647 ≤ tmp_2; */
        r = - tmp_2;
      }
      else 
        if ((long)v2 != -2147483648) {
          int tmp_3;
          /*@ assert rte_18: signed_overflow: -2147483647 ≤ v1; */
          /*@ assert rte_19: signed_overflow: -2147483647 ≤ v2; */
          q = stb__div(- v1,- v2);
          /*@ assert rte_20: signed_overflow: -2147483647 ≤ v1; */
          /*@ assert rte_21: signed_overflow: -2147483647 ≤ v2; */
          tmp_3 = stb__mod(- v1,- v2);
          /*@ assert rte_22: signed_overflow: -2147483647 ≤ tmp_3; */
          r = - tmp_3;
        }
        else {
          q = 1;
          /*@ assert rte_23: signed_overflow: -2147483648 ≤ q * v2; */
          /*@ assert rte_24: signed_overflow: q * v2 ≤ 2147483647; */
          /*@ assert
        rte_25: signed_overflow: -2147483648 ≤ v1 - (int)(q * v2);
          */
          /*@ assert rte_26: signed_overflow: v1 - (int)(q * v2) ≤ 2147483647;
          */
          r = v1 - q * v2;
        }
    else 
      if (v2 >= 0) {
        int tmp_4;
        int tmp_5;
        /*@ assert rte_27: signed_overflow: -2147483648 ≤ v1 + v2; */
        /*@ assert rte_28: signed_overflow: v1 + v2 ≤ 2147483647; */
        /*@ assert rte_29: signed_overflow: -2147483647 ≤ v1 + v2; */
        tmp_4 = stb__div(- (v1 + v2),v2);
        /*@ assert rte_30: signed_overflow: -2147483647 ≤ tmp_4; */
        /*@ assert rte_31: signed_overflow: -2147483648 ≤ (int)(-tmp_4) - 1;
        */
        q = - tmp_4 - 1;
        /*@ assert rte_32: signed_overflow: -2147483648 ≤ v1 + v2; */
        /*@ assert rte_33: signed_overflow: v1 + v2 ≤ 2147483647; */
        /*@ assert rte_34: signed_overflow: -2147483647 ≤ v1 + v2; */
        tmp_5 = stb__mod(- (v1 + v2),v2);
        /*@ assert rte_35: signed_overflow: -2147483647 ≤ tmp_5; */
        r = - tmp_5;
      }
      else 
        if ((long)v2 != -2147483648) {
          int tmp_6;
          int tmp_7;
          /*@ assert rte_36: signed_overflow: -2147483648 ≤ v1 - v2; */
          /*@ assert rte_37: signed_overflow: v1 - v2 ≤ 2147483647; */
          /*@ assert rte_38: signed_overflow: -2147483647 ≤ v1 - v2; */
          /*@ assert rte_39: signed_overflow: -2147483647 ≤ v2; */
          tmp_6 = stb__div(- (v1 - v2),- v2);
          /*@ assert rte_40: signed_overflow: tmp_6 + 1 ≤ 2147483647; */
          q = tmp_6 + 1;
          /*@ assert rte_41: signed_overflow: -2147483648 ≤ v1 - v2; */
          /*@ assert rte_42: signed_overflow: v1 - v2 ≤ 2147483647; */
          /*@ assert rte_43: signed_overflow: -2147483647 ≤ v1 - v2; */
          /*@ assert rte_44: signed_overflow: -2147483647 ≤ v2; */
          tmp_7 = stb__mod(- (v1 - v2),- v2);
          /*@ assert rte_45: signed_overflow: -2147483647 ≤ tmp_7; */
          r = - tmp_7;
        }
        else {
          q = 1;
          r = 0;
        }
  if (r >= 0) {
    __retres = q;
    goto return_label;
  }
  else {
    int tmp_8;
    if (v2 > 0) tmp_8 = -1; else tmp_8 = 1;
    ;
    /*@ assert rte_46: signed_overflow: -2147483648 ≤ q + tmp_8; */
    /*@ assert rte_47: signed_overflow: q + tmp_8 ≤ 2147483647; */
    __retres = q + tmp_8;
    goto return_label;
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
requires v2 > -2147483647;
assigns \nothing;
ensures (v1 >= 0 && v2 > 0) ==> (0 <= \result < v2);
ensures (v1 >= 0 && v2 < 0) ==> (0 <= \result < -v2);
ensures (v1 < 0 && v2 > 0) ==> (-v2 < \result <= 0);
ensures (v1 < 0 && v2 < 0) ==> (v2 < \result <= 0);
*/
int stb_mod_trunc(int v1, int v2)
{
  int __retres;
  if (v1 >= 0) {
    int r = stb__mod(v1,v2);
    if (r >= 0) {
      __retres = r;
      goto return_label;
    }
    else {
      int tmp_0;
      if (v2 < 0) tmp_0 = v2;
      else 
        /*@ assert rte_68: signed_overflow: -2147483647 ≤ v2; */
        tmp_0 = - v2;
      ;
      /*@ assert rte_69: signed_overflow: -2147483648 ≤ r - tmp_0; */
      /*@ assert rte_70: signed_overflow: r - tmp_0 ≤ 2147483647; */
      __retres = r - tmp_0;
      goto return_label;
    }
  }
  else {
    int r_0 = stb__mod(v1,v2);
    if (r_0 <= 0) {
      __retres = r_0;
      goto return_label;
    }
    else {
      int tmp_2;
      if (v2 < 0) tmp_2 = v2;
      else 
        /*@ assert rte_71: signed_overflow: -2147483647 ≤ v2; */
        tmp_2 = - v2;
      ;
      /*@ assert rte_72: signed_overflow: -2147483648 ≤ r_0 + tmp_2; */
      /*@ assert rte_73: signed_overflow: r_0 + tmp_2 ≤ 2147483647; */
      __retres = r_0 + tmp_2;
      goto return_label;
    }
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
assigns \nothing;
ensures (v2 > 0) ==> (\result >= 0 && \result < v2);
ensures (v2 < 0) ==> (\result <= 0 && \result > v2);
*/
int stb_mod_floor(int v1, int v2)
{
  int __retres;
  if (v2 >= 0) {
    int r = stb__mod(v1,v2);
    if (r >= 0) {
      __retres = r;
      goto return_label;
    }
    else {
      /*@ assert rte_64: signed_overflow: -2147483648 ≤ r + v2; */
      /*@ assert rte_65: signed_overflow: r + v2 ≤ 2147483647; */
      __retres = r + v2;
      goto return_label;
    }
  }
  else {
    int r_0 = stb__mod(v1,v2);
    if (r_0 <= 0) {
      __retres = r_0;
      goto return_label;
    }
    else {
      /*@ assert rte_66: signed_overflow: -2147483648 ≤ r_0 + v2; */
      /*@ assert rte_67: signed_overflow: r_0 + v2 ≤ 2147483647; */
      __retres = r_0 + v2;
      goto return_label;
    }
  }
  return_label: return __retres;
}

/*@
requires v2 != 0;
requires v2 != -2147483648;
assigns \nothing;
ensures \result >= 0;
ensures v2 > 0 ==> \result < v2;
ensures v2 < 0 ==> \result < -v2;
*/
int stb_mod_eucl(int v1, int v2)
{
  int __retres;
  int r = stb__mod(v1,v2);
  if (r >= 0) {
    __retres = r;
    goto return_label;
  }
  else {
    int tmp_0;
    if (v2 < 0) tmp_0 = v2;
    else 
      /*@ assert rte_61: signed_overflow: -2147483647 ≤ v2; */
      tmp_0 = - v2;
    ;
    /*@ assert rte_62: signed_overflow: -2147483648 ≤ r - tmp_0; */
    /*@ assert rte_63: signed_overflow: r - tmp_0 ≤ 2147483647; */
    __retres = r - tmp_0;
    goto return_label;
  }
  return_label: return __retres;
}