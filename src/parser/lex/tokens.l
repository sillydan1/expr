m4_changequote()

%{
 // TODO: Remove [ðđ€\(\)]
%}
id     [a-z_A-Z]([.ðđ€\(\)a-zA-Z_0-9]*[a-zA-Z_0-9]+)?
int    [0-9]+[Ll]?
flt    [0-9]+[.][0-9]+[fd]?
bool   [Ff]alse|[Tt]rue
str    \"(\\.|[^\\"])*\"
blank  [ \t\r]
accmod [Pp](ublic|rivate|rotected)
type   int|long|float|double|string|bool|var|auto
