10
{
start
Empty
1 -1
157 95
207 135
}
{
read
a,b,c
2 -1
307 92
377 132
}
{
expression
delta = b^2 - 4*a*c
5 -1
430 157
562 197
}
{
stop
Empty
-1 -1
169 730
219 770
}
{
write
((0-b)+sqrt(delta))/2*a
7 -1
202 473
530 513
}
{
if_statement
delta == 0
6 8
258 234
404 274
}
{
write
(0-b)/2*a
3 -1
139 316
269 356
}
{
write
((0-b)-sqrt(delta))/2*a
3 -1
206 597
526 637
}
{
if_statement
delta > 0
4 9
475 309
601 349
}
{
write
"nu exista"
3 -1
622 685
778 725
}
