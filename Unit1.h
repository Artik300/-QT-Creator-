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

/* тип перечисления для обозначения результата прасинга строки:
undef - неопознанное значение, neop - недостаточно операндов,
val - занесено новое значение, add - cложение, sub - вычитание,
mul - умножение, divi - деление */
enum ParseResult{undef=-2, neop=-1, val=0, add=1, sub=2, mul=3, divi=4};

using namespace std;

//объявление функций
void putBinOpToOutputStr(char, string&, Stack<char>*);//добавить юинарную операцию в стек
void putMulSignBefBrack(string&);//расставить в строки знаки умножения перед круглой скобкой
void deleteSpaces(string&);//убрать пробелы
void makePolis(string, string&);//создать польскую заптсь из строки
int parseToCalc(string, Stack<double>*);//вычислить выражение
int operPrior(char);//приоритет опреации
string findSubstrBySpace(string &str);//поиск строки до первого пробела
string findSubstrBySign(string &str);//поиск строки до первого знака операции

template<class T> //шаблонная струтура для описания узла стека
struct Node{
T data;//ключевое поле
Node *next;//указатель на следующий элемент
Node(T data){this->data=data; this->next=nullptr;}//конструктор первого узла
};

template<class T> //шаблонный класс, описывающий стек
class Stack{
Node<T> *_top;//вершина стека (закрытое поле)
public:
Stack(){_top=nullptr;}//конструктор по умолчанию
void push(T);//поместить элемент в вершину стека
T pop(void);//извлечь элемент из вершины стека
string print();// напечатать стек
int size();//узнать размер стека
bool isNotEmpty(){return (_top)?true:false;}//проверка пустоты стека
T topValue();//просмотреть вершину стека без извлеченеия
};

template<class T>
void Stack<T>::push(T value){
Node<T> *newItem = new Node<T>(value);//создаем новый элемент с заданым значением
if(_top)//если стек не пустой
	newItem->next=_top;//следующий для созданного - вершина стека
_top=newItem;//меняем вершину стека
}

template<class T>
T Stack<T>::pop(void){
if(_top){//если стек не пустой
	T value=_top->data; //запоминаем значение из вершины
	Node<T> *ptr = _top;//создаем копию указателя на верхушку стека
	_top = _top->next; //смещаем вешину на следующий элемент
	delete ptr;//освобождаем память по адресу вершины
	return value;// возвращаем значение
	}
throw string("stack is empty");//исключение, что из пустого стека нельзя извлекать
return 0;//для комплятора, т.к. функция не void
}

template<class T>
string Stack<T>::print(){
stringstream out;
Node<T> *ptr=_top;
while(ptr){//пока не дошли до конца стека
	out<<ptr->data<<' ';//выводим в строковый поток значение в узле и пробел
	ptr=ptr->next;//переходим к следующему узлу
	}
return out.str();//формируем и возвращаем строку из строкового потока
}

template <class T>
int Stack<T>::size(){
Node<T> *ptr=_top;
int count=0;//счетчик узлов
while(ptr){//пока не дошли до конца стека
	count++;//увеличиваем счетчик
	ptr=ptr->next;
	}
return count;
}

template <class T>
T Stack<T>::topValue(){
return _top->data;//возвращаем значение в вершине
}
