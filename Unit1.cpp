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
Label1->Caption="����� ����� �����";
Stack<double> *stackValues = new Stack<double>; //���� ��������
string outstr, instr;
AnsiString s=Edit1->Text;
instr=s.c_str();// ������� ������ �� Edit
deleteSpaces(instr);//������� �������
putMulSignBefBrack(instr);//���������� ����� ��������� ����� ��������
try{
makePolis(instr,outstr);//��������� �������� ������, � ��� �������� � ����� ��������� ���������
string substr = findSubstrBySpace(outstr);//���� ��������� �� ������� �������
while(substr.length()!=0){//���� ��������� �� ������
	switch (parseToCalc(substr,stackValues)) {//�������� ��� � ���, ���� ��������, �� ������� � ���� ��������
		case val: break;//��������
		case add:{//�����
			double tmp=stackValues->pop()+stackValues->pop();//��������� �� ����� ��� ��������� ��������
			stackValues->push(tmp);//������� ��������� ������������ � ����
			break;
			}
		case sub:{
			double tmp2=stackValues->pop();//������ �������
			double tmp1=stackValues->pop();//������
			stackValues->push(tmp1-tmp2);//������� ��������� � ����
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
				throw string("can`t divide by zero");//������� ���������� ������� �� 0
				break;
				}
			stackValues->push(tmp1/tmp2);
			break;
			}
		case neop:{
			throw string("not enough opernads");//������� ���������� � ��������������� ���������
			break;
			}
		case undef:{
			throw string("undefined argument or operation sign");//������� ���������� � �������������� �������� ��� ����� ��������
			break;
			}
		}
	substr = findSubstrBySpace(outstr);//�������������� ��������� ������ ��������� �� �������
	}
if(stackValues->size()==1)//���� � ����� ����� ���� �������� - ��� � �������
Label1->Caption=stackValues->print().c_str();//������� ���������
}
catch(string x){//������������ �������������� ��������
ShowMessage(x.c_str());
}

}
//---------------------------------------------------------------------------


void putBinOpToOutputStr(char sign, string &outstr, Stack<char> *stack){
//���� ���� �� ������ � ��������� ������� �������� ������ ���������� �������� �� �������� �����
while (stack->isNotEmpty()&&operPrior(stack->topValue())>operPrior(sign)){
	char symbol = stack->pop();//��������� ���� �������� �� �����
	string buff = {symbol,' '};
	outstr.append(buff);//��������� ���� �������� � ������ � �������� ������
	}
stack->push(sign);//�������� ������� �������� � ����
}

void putMulSignBefBrack(string &str){
int i=0,pos=str.find("(",i);
while(pos!=-1){ //���� � ������ ��������� ������, ������� � ������� i
//���� ������ ����, ��� �� ������, ����� ��� ����� ��� ����������� ������
  if(pos>0&&(isdigit(str[pos-1]))||str[pos-1]==')')
	{str.insert(pos,"*");pos++;}//�������� ���� ��������� � ������� ������� ����� �� 1
  i=pos+1;
  pos=str.find('(',i);//������� ������� ����������� ������� ������,������� � ������� i
  }
}

void deleteSpaces(string &str){
int pos=str.find(' ');//������� ������ ������
while(pos!=-1){ //���� ������� ����
   str.erase(pos,1);//�������
   pos=str.find(' ');//������� ����� ������
   }
}

void makePolis(string instr, string &outstr){//������������ ����� �� ���������� ������ ���������� ������������� �������
Stack <char> *operations = new Stack<char>;//���� ������ ��������
bool dflt;//������� ��������, � �� ��������
while(instr.length() > 0){//���� � ������ ���� �������
	dflt = false;//��������������� ������� - �� ��������, � ��������
	switch (instr.at(0)) {//������� ������ ������ �������� ������
		case '+':
			putBinOpToOutputStr('+',outstr,operations);//������������ �������� �������� +
			break;
		case '-':
			putBinOpToOutputStr('-',outstr,operations);//������������ �������� �������� -
			break;
		case '*':
			putBinOpToOutputStr('*',outstr,operations);//������������ �������� �������� *
			break;
		case '/':
			putBinOpToOutputStr('/',outstr,operations);//������������ �������� �������� /
			break;
		case '(':
			operations->push('('); //����������� ������ ������ ��������� � ����
			break;
		case ')':{
			while(operations->isNotEmpty()&&operations->topValue()!='('){//���� ���� �� ������ � ���� �� ����������� ����������� ������
				char symbol = operations->pop();//��������� ���� ��������
				string buff = {symbol,' '};
				outstr.append(buff);//��������� ����������� ���� � �������������� ������
				}
			if(operations->isNotEmpty())//���� ���� �� ������
				operations->pop();//��������� ����������� ������
			else
				throw string("not right order of brackets");//���� ���� � ����������� ������ �� ����, �� ������� ����������
			break;
			}
		default://� ������ ������ ��������
			dflt = true;//�������������� ����
			outstr.append(findSubstrBySign(instr));//� �������������� ������ ���������� �������� �� ���������� ����� ��������
			outstr += " ";//���������� ������

		}
	if(!dflt)//���� � ������ �� ��������, ��
		instr.erase(0,1);//������� �� �������� ������ ���� ��������
	}
//�������� ������ ������
while(operations->isNotEmpty()){//���� ���� �� ����, �������� � �������������� ������ ���������� � ����� ��������
	char s = operations->pop();
	if(s!='('){
		string sign={s,' '};
		outstr.append(sign);
		}
	else {//���� � ����� ����������� ����������� ������
	   throw string("left bracket without right");//������� ����������
	   }
	}
outstr.erase(outstr.length()-1,1);//����� ������ �������� � �������� � �������������� ������ ������������ ������, ��������� ������ �������
}

int parseToCalc(string str, Stack <double>* stack){//������� ������� ���������, ���������� ������, ���� ���������
double value = 0;//��������
switch (str.at(0)) {//������� ������ ������ ������
	case '-'://���� �����
		if (str.length()>1) break;//������ ��� �� ��������, � ���� �����
		if (stack->size() >= 2)//� ����� ���������� ���������, � ������� ��������� ��������
			return sub;//���������� ������� ���������
		else
			return neop;//������������ ���������
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
value = atof(str.c_str());//���� �� ��������, ��������� �� ������ �����
stack->push(value);//�������� ��� � ����
return val;//���������� ������� ��������
}

int operPrior(char sign){//���������� ��������� ��������
switch(sign){
	case '+':
		return 1;
	case '-':
		return 2;//��� ��������� ��������� ����� ������� ��������� ����� ��������� ������ ��������
	case '*':
		return 3;
	case '/':
		return 4;
	case '(':
	case ')':
		return 0;
	}
throw "invalid iperation";//�� ��������������� ���� ��������
//return -1;
}

string findSubstrBySpace(string &str){//���������� ��������� �� ������� �������, �� ������� ������ ������� ���������
string strcopy=str;//����� ������� ������
int pos = str.find(' ');//���� ������
if (pos!=-1){//������ ����
	strcopy.erase(pos);//������� �� ����� ��� �� ������� �� ����� ������
	str.erase(0,strcopy.length()+1);//������� �� �������� ������ ��� �� ������� �������, � ��� ������
	}
else
	str.clear();//���� �������� ���, ������� ������
return strcopy;//���������� �����
}

string findSubstrBySign(string &str){//���������� ��������� �� ������� ����� ��������, �� ������� ������ ������� ���������
string strcopy = str;
unsigned int pos = str.find_first_of("+-/*()");
if (pos!=string::npos)
	strcopy.erase(pos);
str.erase(0,strcopy.length());
return strcopy;
}



