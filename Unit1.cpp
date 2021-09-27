//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
55*5-34(3-4)/34 + 22 - 42*34(2-3)
1726
*/


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
Label1->Caption="здесь будет ответ";
Stack<double> *stackValues = new Stack<double>; //стек значений
string outstr, instr;
AnsiString s=Edit1->Text;
instr=s.c_str();// входная строка из Edit
deleteSpaces(instr);//удаляем пробелы
putMulSignBefBrack(instr);//доставляем знаки умножения перед скобками
try{
makePolis(instr,outstr);//формируем польскую запись, в ней значение и знаки разделены пробелами
string substr = findSubstrBySpace(outstr);//ищем подстроку до первого пробела
while(substr.length()!=0){//пока подстрока не пустая
	switch (parseToCalc(substr,stackValues)) {//выясняем что в ней, если значение, то заносим в стек значений
		case val: break;//значение
		case add:{//сумма
			double tmp=stackValues->pop()+stackValues->pop();//извлекаем из стека два последних значения
			stackValues->push(tmp);//заносим результат суммирования в стек
			break;
			}
		case sub:{
			double tmp2=stackValues->pop();//второй операнд
			double tmp1=stackValues->pop();//первый
			stackValues->push(tmp1-tmp2);//заносим результат в стек
			break;
			}
		case mul:{
			double tmp=stackValues->pop()*stackValues->pop();
			stackValues->push(tmp);
			break;
			}
		case divi:{
			double tmp2=stackValues->pop();
			double tmp1=stackValues->pop();
			if(!tmp2){
				throw string("can`t divide by zero");//создаем исключение деления на 0
				break;
				}
			stackValues->push(tmp1/tmp2);
			break;
			}
		case neop:{
			throw string("not enough opernads");//создаем исключение о недостаточности операндов
			break;
			}
		case undef:{
			throw string("undefined argument or operation sign");//создаем исключение о неопределенном операнде или знаке операции
			break;
			}
		}
	substr = findSubstrBySpace(outstr);//перезаписываем подстроку первым значением до пробела
	}
if(stackValues->size()==1)//если в стеке всего одно значение - все в порядке
Label1->Caption=stackValues->print().c_str();//выводим результат
}
catch(string x){//обрабатываем исключительные ситуации
ShowMessage(x.c_str());
}

}
//---------------------------------------------------------------------------


void putBinOpToOutputStr(char sign, string &outstr, Stack<char> *stack){
//пока стек не пустой и приоритет текущей операции меньше приоритета операции на верхушке стека
while (stack->isNotEmpty()&&operPrior(stack->topValue())>operPrior(sign)){
	char symbol = stack->pop();//извлекаем знак операции из стека
	string buff = {symbol,' '};
	outstr.append(buff);//добавляем знак операции и пробел к выходной строке
	}
stack->push(sign);//помещаем текущую операцию в стек
}

void putMulSignBefBrack(string &str){
int i=0,pos=str.find("(",i);
while(pos!=-1){ //пока в строке находится скобка, начиная с позиции i
//если скобка есть, она не первая, перед ней цифра или закрывающая скобка
  if(pos>0&&(isdigit(str[pos-1]))||str[pos-1]==')')
	{str.insert(pos,"*");pos++;}//всавляем занк умножения и смещаем позицию поска на 1
  i=pos+1;
  pos=str.find('(',i);//находим позицию открывающей круглой скобки,начиная с позиции i
  }
}

void deleteSpaces(string &str){
int pos=str.find(' ');//находим первый пробел
while(pos!=-1){ //пока пробелы есть
   str.erase(pos,1);//удаляем
   pos=str.find(' ');//находим новый пробел
   }
}

void makePolis(string instr, string &outstr){//формирование Полиз их инфниксной записи алгоритмом сортировочной станции
Stack <char> *operations = new Stack<char>;//стек знаков операций
bool dflt;//признак значения, а не операции
while(instr.length() > 0){//пока в строке есть символы
	dflt = false;//рассматриваемый элемент - не значение, а операция
	switch (instr.at(0)) {//смотрим первый символ исходной строки
		case '+':
			putBinOpToOutputStr('+',outstr,operations);//обрабатываем бинарную операцию +
			break;
		case '-':
			putBinOpToOutputStr('-',outstr,operations);//обрабатываем бинарную операцию -
			break;
		case '*':
			putBinOpToOutputStr('*',outstr,operations);//обрабатываем бинарную операцию *
			break;
		case '/':
			putBinOpToOutputStr('/',outstr,operations);//обрабатываем бинарную операцию /
			break;
		case '(':
			operations->push('('); //открывающая скобка просто заносится в стек
			break;
		case ')':{
			while(operations->isNotEmpty()&&operations->topValue()!='('){//пока стек не пустой и пока не встретиться открывающая скобка
				char symbol = operations->pop();//извлекаем знак операции
				string buff = {symbol,' '};
				outstr.append(buff);//добавляем полученнный знак в резулютирующую строку
				}
			if(operations->isNotEmpty())//если стек не пустой
				operations->pop();//извлекаем открывающую скобку
			else
				throw string("not right order of brackets");//стек пуст и открывающей скобки не было, то создаем исключение
			break;
			}
		default://в строке сейчас значение
			dflt = true;//устанавиливаем флаг
			outstr.append(findSubstrBySign(instr));//в результирующую строку записываем значение до ближайшего знака операции
			outstr += " ";//дописываем пробел

		}
	if(!dflt)//если в строке не значение, то
		instr.erase(0,1);//убираем из исходной строки знак операции
	}
//исходная строка пустая
while(operations->isNotEmpty()){//если стек не пуст, помещаем в результирующую строку оставшиеся в стеке операции
	char s = operations->pop();
	if(s!='('){
		string sign={s,' '};
		outstr.append(sign);
		}
	else {//если в стеке встретилась открывающая скобка
	   throw string("left bracket without right");//создаем исключение
	   }
	}
outstr.erase(outstr.length()-1,1);//после каждой операции и значения в результирующую строку дописывается пробел, последний пробел удаляем
}

int parseToCalc(string str, Stack <double>* stack){//функция получаю подстроку, являющуюся знаком, либо значением
double value = 0;//значение
switch (str.at(0)) {//смотрим первый символ строки
	case '-'://если минус
		if (str.length()>1) break;//значит это не операция, а знак числа
		if (stack->size() >= 2)//в стеке достаточно операндов, к которым применить операцию
			return sub;//возвращаем признак вычитания
		else
			return neop;//недостаточно операндов
	case '+':
		if (stack->size() >= 2)
			return add;
		else
			return neop;
	case '*':
		if (stack->size() >= 2)
			return mul;
		else
			return neop;
	case '/':
		if (stack->size() >= 2)
			return divi;
		else
			return neop;
	}
value = atof(str.c_str());//если не операция, извлекаем из строки число
stack->push(value);//помещаем его в стек
return val;//возвращаем признак значения
}

int operPrior(char sign){//определяем приоритет операции
switch(sign){
	case '+':
		return 1;
	case '-':
		return 2;//при просмотре выражения слева направо вычитание нужно выполнить раньше сложения
	case '*':
		return 3;
	case '/':
		return 4;
	case '(':
	case ')':
		return 0;
	}
throw "invalid iperation";//не предусмотренный знак операций
//return -1;
}

string findSubstrBySpace(string &str){//возвращает подстроку до первого пробела, из текущей строки удаляет найденную
string strcopy=str;//копия текущей строки
int pos = str.find(' ');//ищем пробел
if (pos!=-1){//пробел есть
	strcopy.erase(pos);//удаляем из копии все от пробела до конца строки
	str.erase(0,strcopy.length()+1);//удаляем из исходной строки все до первого пробела, и сам пробел
	}
else
	str.clear();//если пробелов нет, очищаем строку
return strcopy;//возвращаем копию
}

string findSubstrBySign(string &str){//возвращает подстроку до первого знака операции, из текущей строки удаляет найденную
string strcopy = str;
unsigned int pos = str.find_first_of("+-/*()");
if (pos!=string::npos)
	strcopy.erase(pos);
str.erase(0,strcopy.length());
return strcopy;
}



