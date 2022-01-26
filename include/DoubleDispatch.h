#pragma once

#include "utils.h"

class FirstClassObject;
class SecondClassObject;
class Interactor;

class ObjectInteractor {
public:
	virtual void interact(FirstClassObject* object) = 0;
	virtual void interact(SecondClassObject* object) = 0;
};

class InteractorSolver : public ObjectInteractor {
private:
	Interactor* interactor = nullptr;
public:
	InteractorSolver(Interactor* interactor) : interactor{ interactor } {}

	void interact(FirstClassObject* object) override {
		LOG("Interacted with FirstClassObject");
	}

	void interact(SecondClassObject* object) override {
		LOG("Interacted with SecondClassObject");
	}
};

class Object {
public:
	virtual void interactWith(ObjectInteractor* interactor) = 0;
};

class FirstClassObject : public Object {
public:
	void interactWith(ObjectInteractor* interactor) override {
		interactor->interact(this);
	}
};

class SecondClassObject : public Object {
public:
	void interactWith(ObjectInteractor* interactor) override {
		interactor->interact(this);
	}
};

class Interactor {
	InteractorSolver solver;
public:
	Interactor() : solver(this) {}
	friend InteractorSolver;
	void interactWith(Object* object) {
		object->interactWith(&solver);
	}
};



