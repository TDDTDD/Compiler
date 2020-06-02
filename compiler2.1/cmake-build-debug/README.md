## Syntax：

* 块的开始结束用void main{ }

* 支持的int,double数据类型,不支持逗号分隔符：例如int i,j; 需要定义成 int i; int j;

* 支持赋值语句

* 支持算数表达式

* 支持布尔表达式

* 支持循环判断repeat until, if then else endif;

* 每行代码结束使用";"

* if条件语句:(语句必须完整)

		if [...] then
            ...;
        else
            ...;
        endif;

* repeat条件语句:

		repeat
            ...;
        until [...];

* 终端输出使用print(),读入使用read():

		print(...,...)
		read(...,...)

	例子

	void main ( ) {
        double tdd;
        tdd=0.0;
        int a; a=1;
        int b; b=1;
        int c;
        repeat
            c=a+b;
            a=b;
            b=c;
            print(b);
        until [b>a+10];
        repeat
            repeat
                if [b>=0] then
                    b=b-1;
                    print(b);
                else
                    b=b+2;
                endif;
            until [b>5+a];
        until [b>2+(a*c)];
    }


























算数表达式  （不单独出现）
A1->B1 A1'
A1'->+B1A1'| -B1A1' | ε
B1->C1 B1'
B1'->*C1B1'| /C1B1' | ε
C1->(A1) | i  | n

(i为标识符identifier，n为数字number)
形如：(a*b+c)*d*2
---------------------------------------------
赋值：	（单独构成一句子）
A2->i=A1;

（i为标识符）
形如：i=2*a+c
---------------------------------------------
布尔表达式(bool)	：  （不单独出现）
布尔表达式在判断中出出现,添加"[ ]"实现区分
A3->[ A1 C3 ]
C3-> <A1 | >A1 | <=A1 | >=A1 | ==A1 | !=A1 | or A1 | and A1


形如：[5*3-1 > 2*4-1]
---------------------------------------------

定义(int,double)：   （单独构成一句子）

A4->int i ;| double i ;
形如：int i;
---------------------------------------------

循环(repeat until)	（单独构成一句子）
A5->repeat A8 until A3 ;

形如：
repeat
	i=i+1;
until [i>5];
---------------------------------------------

判断(if then else endif)(单独构成一句子)

A6  if A3 then A8 else A8 endif ;

形如：
if [i>5] then
	i=i+1;
else
	i=i-1;
endif;
---------------------------------------------
函数： read(a,b)		(单独构成一句子)
A7->read(C7); | print(C7);
C7->D7 C7'
C7'->,D7 C7'| ε
D7->i

形如：
read(i);
print(i);
---------------------------------------------
句子组合：
S->void main { A8 }
A8-> A2(赋值)A8' | A4(定义)A8' |  A5(循环)A8' | A6(判断)A8'| A7(函数)A8'
A8'-> A8 | ε


形如：
if [i>5] then
	int a;
	a=3*4;
	i=i+1;
elseif [i<3]
	i=i-1;
	print(i);
endif;
repeat
	i=i+1;
until [i>5];