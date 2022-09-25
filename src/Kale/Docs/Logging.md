
@page logging Using the Logging System & Printing

Logging can be incredibly useful for debugging and figuring out what's wrong with your code. Unfortunately logging as a whole is somewhat more difficult in C++ over other languages such as Java or Javascript. Kale help's make logging easier by allowing you to log to both the standard output, and log files at the same time, along with printing the line number, file name, and other useful information. To log a custom type, simply create an operator overload to log it to `std::ostream`. The types of logging functions are described below:

 - @ref klPrint - A macro to print anything to the console with the file name and line number.
 - @ref klInfo - A macro to print anything to the console with the file name, line number, and an info label.
 - @ref klWarn - A macro to print anything to the console with the file name, line number, and a warning label.
 - @ref klError - A macro to print anything to the console with the file name, line number, and an error label.
 - @ref klAssert - A macro to assert if a condition is true, if it is false then an error will be printed to the console with the line number and file name.
 - @ref klAssertMsg - Same behavior as klAssert, but also prints an associated error message if the assertion is false.
 - @ref Kale::Logger::log "console.log" - Templated function to print to the console
 - @ref Kale::Logger::info "console.info" - Templated function to print info to the console
 - @ref Kale::Logger::warn "console.warn" - Templated function to print a warning to the console
 - @ref Kale::Logger::error "console.error" - Templated function to print an error to the console

In release mode, logging is only enabled to the log files rather than to the standard output. All of the above functions print to both the standard output and the logging files in debug mode. Logging files can be found in `.ApplicationName/logs/` where `ApplicationName` is the name given in the constructor to @ref Kale::Application "application". All logging functions also are thread safe, they can be called from any thread without interloping or any unexpected behavior.
