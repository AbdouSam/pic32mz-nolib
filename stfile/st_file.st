PROGRAM program0
  VAR
    tmpreset AT %MD0 : REAL := 3.0;
    poweron AT %IX0.0 : BOOL;
    lamp AT %QX0.1 : BOOL;
  END_VAR
  VAR
    TOF0 : TOF;
    REAL_TO_TIME8_OUT : TIME;
  END_VAR

  REAL_TO_TIME8_OUT := REAL_TO_TIME(tmpreset);
  TOF0(IN := poweron, PT := REAL_TO_TIME8_OUT);
  lamp := TOF0.Q;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#10ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
