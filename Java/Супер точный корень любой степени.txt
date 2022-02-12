BigDecimal getRoot(BigDecimal num, int pow, int sc){
      BigDecimal scale = BigDecimal.valueOf(0.1).pow(sc), x, xk = BigDecimal.TEN;
      for(;;){
          x = (xk.multiply(BigDecimal.valueOf(pow-1)).add(num.divide(xk.pow(pow-1), sc+1, BigDecimal.ROUND_FLOOR))).divide(BigDecimal.valueOf(pow), sc+1, BigDecimal.ROUND_FLOOR);
          if(scale.compareTo(x.subtract(xk).abs())>0) break;
          xk = x;
      }
      return x;
}