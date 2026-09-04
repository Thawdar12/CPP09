
/*
read token
↓
is it a valid number?
    ↓ yes → push onto stack
    ↓ no
is it a valid operator?
    ↓ yes → check stack has 2 operands
                 ↓
              calculate
                 ↓
              push result
    ↓ no
error
↓
finished?
↓
stack must contain exactly 1 result
*/
