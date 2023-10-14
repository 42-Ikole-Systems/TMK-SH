add_library(
    lib_rules
    OBJECT
    and_or.cpp
    list_rule.cpp
    separator_op.cpp
	command_name.cpp
	command_prefix.cpp
	command_suffix.cpp
	command_word.cpp
	filename.cpp
	io_file.cpp
	io_redirect.cpp
	simple_command.cpp
	assignment_word.cpp
	word.cpp
	here_end.cpp
	io_here.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_rules>
    PARENT_SCOPE)
