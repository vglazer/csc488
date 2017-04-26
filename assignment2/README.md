# GRAMMAR DESIGN
Our general approach was to simply "bisonify" the reference grammar provided. A 
few left-recursive productions (declaration_list, statement_list and 
output_list) were added to resolve shift/reduce conflicts, and bison's %left, 
%right and %nonassoc declarations, coupled with the %prec modifier, were used 
to specify the precedence and associativity of the various operators (thus
avoiding reduce/reduce conflicts in the expression production).

# TESTING STRATEGY
Since we already spent a considerable amount of time ensuring that our 
Assignment #1 programs tested every aspect of the project language, it made 
sense to reuse as much of that code as possible. Those programs, albeit spliced
together and in a slightly modified form, make up the bulk of our test cases.
The rest were written from scratch: one verifies that "a = b = c" is a valid 
statement (as discussed on the course newsgroup), another verifies that, in a 
given scope, all declarations must precede any statements and the rest are 
intended to demonstrate that the operators declared to be non-associative do in
fact fail to associate, e.g. that a = b = c is not a valid expression (the 
latter regrettably couldn't be combined into a single file, since only the 
first error is reported). Note that parse errors are expected in some cases; 
the testing script identifies these accordingly.

# TESTING INSTRUCTIONS
Simply run `make` (to build the parser), then execute the `runTests` script.
