#include "Poly.h"
#include <iostream>
#include <cmath>


using namespace std;

Poly::Poly()
{
	head = new PolyNode(-1, 0, NULL); //zero polynomial with degree set to -1
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	head = new PolyNode(-1, 0, NULL);
	PolyNode* current = head;
	for (int i = 0; i < size(deg); i++) { //loops through the size of deg
		current->next = new PolyNode(deg[i], coeff[i], NULL); //sets the next node to the polynomial
		current = current->next; //updates current position
	}
}

Poly::~Poly()
{
	while (head->next) { //emptys linked list
		auto tmp = head->next; //sets the current node element to tmp
		head->next = tmp->next; //set the old node to the next next node aka relinking the linkedlist
		delete tmp; //deletes whatever is stored in temp
	}
}

void Poly::addMono(int i, double c) //runtime is big theta(n) and space complexity O(n)
{
	PolyNode* prev = head;
	PolyNode* current = head;
	while (current->deg != i && current->next != NULL && current->next->deg >= i) { //loops through the link list to find correct spot to do stuff (covers all cases)
		prev = current; //set prev to head
		current = current->next; //updates current
	}
	if (current->deg == i) {
		current->coeff += c; //adding coefficient with the current node
		if (current->coeff == 0) {
			prev->next = current->next; //connects pointers so the data isnt lost after we delete it
			delete current; //delete node if the new coefficient is 0
		}
	}
	else {
		current->next = new PolyNode(i, c, current->next); //adding new node if it's not there
	}
}

void Poly::addPoly(const Poly& p) //running time of big theta(n^2) and space complexity of O(n)
{
	PolyNode* temp = p.head->next; //temp is the given polynomial head->next

	while (temp != NULL) {
		addMono(temp->deg, temp->coeff); //calls addmono to add the polynomial
		temp = temp->next;
	}
}

void Poly::multiplyMono(int i, double c) //running time of big theta(n) and space complexity of O(n)
{
	if (c == 0.0) {
		head->next = NULL;
	}
	PolyNode* current = head->next;
	while (current!=NULL) {
		current->coeff *= c; //adding coefficient with the current node
		current->deg += i; //adding degree with the current node
		current = current->next;
	}
}

void Poly::multiplyPoly(const Poly& p) //running time is big theta(n^3) and space complexity of O(n)
{ //the first polynomial is getting saved to original and since "this" keeps changing, copy is created using original
	Poly original = Poly(); //sets original to a dummy node that's zero polynomial and degree -1
	duplicate(original); //duplicate original
	PolyNode* temp = p.head->next; //temp is the given polynomial head->next
	Poly copy = Poly(); //set to dummy node
	if (p.head->next == NULL) { //deals with the special case if p is null
		head->next = NULL;
	}
	if (temp != NULL) {
		multiplyMono(temp->deg, temp->coeff); //calls multiplyMono to add the polynomial
		temp = temp->next;
	}
	while (temp != NULL) {
		original.duplicate(copy); //creates duplicate, copies original to copy
		copy.multiplyMono(temp->deg, temp->coeff); //calls addmono to add the polynomial
		addPoly(copy);
		temp = temp->next;
	}
}

void Poly::duplicate(Poly& outputPoly) //running time is big theta(n) and space complixity of O(n)
{
	PolyNode* current = head->next;
	outputPoly.head = new PolyNode(-1, 0, NULL);//creates dummy node for the duplicate polynomial
	PolyNode* outputcurrent = outputPoly.head;
	while (current!=NULL) {
		outputcurrent->next = new PolyNode(current->deg, current->coeff, NULL);
		current = current->next;
		outputcurrent = outputcurrent->next;
	}
}

int Poly::getDegree() //running time of bigtheta(1) and space complexity of O(1)
{
	if (head->next == NULL) {
		return -1;
	}
	else {
		return head->next->deg;
	}
}

int Poly::getTermsNo() //running time of bigtheta(n) and space complexity of O(n)
{
	int termcount = 0;
	PolyNode* current = head;
	if (current->next == NULL) {
		return 0;
	}
	else {
		while (current->next != NULL) {
			current = current->next;
			if (current->deg == -1) {
				return 0;
			}
			termcount++;
		}
		return termcount;
	}
}

double Poly::evaluate(double x)
{
	PolyNode* current = head->next;
	double result = 0;
	while (current!=NULL) {
		result += current->coeff * (pow(x, current->deg));
		current = current->next;
	}
	return result;
}

std::string Poly::toString()
{
	string polystring = "";
	PolyNode* current = head->next;
	polystring += "degree= " + std::to_string(getDegree()) + ";";
	while (current!=NULL) {
		polystring += " a(" + std::to_string(current->deg) + ")=" + std::to_string(current->coeff) + ";";
		current = current->next;
	}
	return polystring;
}
