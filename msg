Solve the issue #17

we need the merging syntax in doxygen comments.
// CGA_VARIANT .. syntax does not wok well in doxygen syntax.

- 3_recover.pl change the CGA_VARIANT type
    - ```// @CGA_VARIANT:xxxx START```  ->  ```// @CGA_VARIANT_START{xxxx}``` : in C++
    - ``` * @CGA_VARIANT:xxxx START```  ->  ``` * @CGA_VARIANT_START{xxxx}``` : in doxygen comments

	modified:   2_example.cpp.stc
	modified:   3_recover.pl
	modified:   3_working.cpp.data
