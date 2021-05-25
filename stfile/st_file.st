PROGRAM program0
  VAR
    bstart AT %IX0.15 : BOOL;
    bstop AT %IX0.14 : BOOL;
    led AT %QX0.15 : BOOL;
    var_byte AT %MB0 : SINT;
    var_long AT %ML0 : ULINT;
  END_VAR

  led := NOT(bstop) AND (bstart OR led);
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#1ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
