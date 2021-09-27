//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

#include<string.h>
#include<sstream>

template<class T>
struct Node;

template<class T>
class Stack;

/* ��� ������������ ��� ����������� ���������� �������� ������:
undef - ������������ ��������, neop - ������������ ���������,
val - �������� ����� ��������, add - c�������, sub - ���������,
mul - ���������, divi - ������� */
enum ParseResult{undef=-2, neop=-1, val=0, add=1, sub=2, mul=3, divi=4};

using namespace std;

//���������� �������
void putBinOpToOutputStr(char, string&, Stack<char>*);//�������� �������� �������� � ����
void putMulSignBefBrack(string&);//���������� � ������ ����� ��������� ����� ������� �������
void deleteSpaces(string&);//������ �������
void makePolis(string, string&);//������� �������� ������ �� ������
int parseToCalc(string, Stack<double>*);//��������� ���������
int operPrior(char);//��������� ��������
string findSubstrBySpace(string &str);//����� ������ �� ������� �������
string findSubstrBySign(string &str);//����� ������ �� ������� ����� ��������

template<class T> //��������� �������� ��� �������� ���� �����
struct Node{
T data;//�������� ����
Node *next;//��������� �� ��������� �������
Node(T data){this->data=data; this->next=nullptr;}//����������� ������� ����
};

template<class T> //��������� �����, ����������� ����
class Stack{
Node<T> *_top;//������� ����� (�������� ����)
public:
Stack(){_top=nullptr;}//����������� �� ���������
void push(T);//��������� ������� � ������� �����
T pop(void);//������� ������� �� ������� �����
string print();// ���������� ����
int size();//������ ������ �����
bool isNotEmpty(){return (_top)?true:false;}//�������� ������� �����
T topValue();//����������� ������� ����� ��� �����������
};

template<class T>
void Stack<T>::push(T value){
Node<T> *newItem = new Node<T>(value);//������� ����� ������� � ������� ���������
if(_top)//���� ���� �� ������
	newItem->next=_top;//��������� ��� ���������� - ������� �����
_top=newItem;//������ ������� �����
}

template<class T>
T Stack<T>::pop(void){
if(_top){//���� ���� �� ������
	T value=_top->data; //���������� �������� �� �������
	Node<T> *ptr = _top;//������� ����� ��������� �� �������� �����
	_top = _top->next; //������� ������ �� ��������� �������
	delete ptr;//����������� ������ �� ������ �������
	return value;// ���������� ��������
	}
throw string("stack is empty");//����������, ��� �� ������� ����� ������ ���������
return 0;//��� ����������, �.�. ������� �� void
}

template<class T>
string Stack<T>::print(){
stringstream out;
Node<T> *ptr=_top;
while(ptr){//���� �� ����� �� ����� �����
	out<<ptr->data<<' ';//������� � ��������� ����� �������� � ���� � ������
	ptr=ptr->next;//��������� � ���������� ����
	}
return out.str();//��������� � ���������� ������ �� ���������� ������
}

template <class T>
int Stack<T>::size(){
Node<T> *ptr=_top;
int count=0;//������� �����
while(ptr){//���� �� ����� �� ����� �����
	count++;//����������� �������
	ptr=ptr->next;
	}
return count;
}

template <class T>
T Stack<T>::topValue(){
return _top->data;//���������� �������� � �������
}
