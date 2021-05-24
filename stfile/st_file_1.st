PROGRAM program0
  VAR
    count AT %IX0.0 : BOOL;
    r_count AT %IX0.1 : BOOL;
    count_reach AT %QX0.0 : BOOL;
  END_VAR
  VAR
    curr_count : INT;
    CTU0 : CTU;
    R_TRIG1 : R_TRIG;
    R_TRIG2 : R_TRIG;
    R_TRIG3 : R_TRIG;
  END_VAR

  R_TRIG1(CLK := count);
  R_TRIG2(CLK := R_TRIG1.Q);
  R_TRIG3(CLK := r_count);
  CTU0(CU := R_TRIG2.Q, R := R_TRIG3.Q, PV := 5);
  count_reach := CTU0.Q;
  curr_count := CTU0.CV;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
