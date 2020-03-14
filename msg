support sldd & %+<+$API+>+ and restore $g_y policy (plus)

v1.0.4
- support sldd 
- support ```ITERATE %+<+$API+>+ ``` syntax
    - add ```$iterate_var_name = replace_var_with_value($iterate_var_name);```
- restore $g_y policy (plus)
    - replace_var_with_value does not break becasue break contents to improve speed  is impossible to keep the order when we count with plus().

