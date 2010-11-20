template <class T>
struct Node
{
	T data;
	Node<T> *next;
	Node(T d, Node<T>* f) : data(d), next(f) {}
};

template <class T>
class Stack
{
private:
	Node<T> *first;
	unsigned size;
public:
	Stack();
	~Stack();
	Stack(const Stack<T> &);
	Stack<T> &operator=(const Stack<T> &);
	void remove();
	void copy(const Stack<T>&);
	T top();
	unsigned getSize() const;
	void reverse();
	bool push(const T&);
	bool pop(T&);
	bool isEmpty();
};

//end stack definition

template <class T>
void Stack<T>::reverse()
{
	Stack<T> temp;
	T data;
	while (this->pop(data)) {
		temp.push(data);
	}
	*this = temp;
}

template <class T>
unsigned Stack<T>::getSize() const
{
	return this->size;
}

template <class T>
T Stack<T>::top()
{
	return first->data;
}

template <class T>
bool Stack<T>::isEmpty()
{
	return first == NULL;
}

template <class T>
Stack<T>::Stack()
{
	first = NULL;
	size = 0;
}

template <class T>
void Stack<T>::remove()		//method which i can use in operator = definition and ~Stack()
{
	Node<T> *tmp;
	while (first) {
		tmp = first;
		first = first->next;
		delete [] tmp;
	}
}

template <class T>
bool Stack<T>::push(const T& data)	//method push
{
	Node<T>* info = new Node<T>(data, first);
	if (info == NULL)
		return false;
	first = info;
	size++;
	return true;
}

//end method push definition

template <class T>
Stack<T>::~Stack()	//detructor
{
	remove();
}


template <class T>
bool Stack<T>::pop(T& returnData)	//taking the first value of the stack
{
	Node<T> *tmp;
	if (first == NULL)
		return false;
	returnData = first->data;
	tmp = first;
	first = first->next;
	delete [] tmp;
	size--;
	return true;
}

template <class T>
Stack<T>::Stack(const Stack<T> &stack)	//copy constructor
{
	copy(stack);
}

template <class T>
Stack<T>& Stack<T>::operator =(const Stack<T>& stack)	//operator =
{														//using the upper methods
	if (this != &stack) {
		this->remove();
		this->copy(stack);
	}
	return *this;
}

template <class T>
void Stack<T>::copy(const Stack<T> &stack)	//copying all stack
{
	Node<T> *tmp = stack.first;
	Node<T> *cpyTmp;
	if (tmp != NULL) {
		Node<T> *data = new Node<T>(tmp->data, tmp->next);
		first = data;
		cpyTmp = first;
		tmp = tmp->next;
		while (tmp) {
			Node<T> *data = new Node<T>(tmp->data, tmp->next);
			cpyTmp->next = data;
			cpyTmp = cpyTmp->next;
			tmp = tmp->next;
		}
	}
}