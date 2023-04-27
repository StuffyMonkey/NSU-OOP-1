#pragma once
#include <string>
#include "Container.h"

class Object {
    public:

	    Object(){ ++numEntities; }
	    virtual ~Object(){ --numEntities; }

        virtual std::string toString() const = 0;
        inline static int numEntities = 0;
};

class Task: public virtual Object {
    public:
	    virtual void run() = 0;
	    virtual bool hasResult() {return isCompleted;}
        bool isCompleted = false;
    protected:
        double result;
};

class Named: public virtual Object {
    public:
        Named(std::string name) : name(name) {}

        std::string toString() {
            return name;
        }
    protected:
        std::string name;
};

class BinaryOperationTask: public Task, public Named {
    public:
	    BinaryOperationTask(std::string const& name, double left, double right, char operation_sign): 
			Named(name),
		    left(left),
		    right(right),
		    operation_sign(operation_sign) {}

	void run() override {
		if (isCompleted) throw "The task was copleted before";
		switch (operation_sign)
		{
		case '+':
			result = left + right;
			break;
		case '-':
			result = left - right;
			break;
		case '/':
			result = left / right;
			break;
		case '*':
			result = left * right;
			break;
		default:
			throw "Unknown binary operation recieved";
		}
		isCompleted = true;
	}

	std::string toString() const override{
		return name + ": " + std::to_string(left) + ' ' + operation_sign + ' ' + std::to_string(right) + " = " +
                                            (isCompleted ? std::to_string(result) : "UNKNOWN") + ' ' +
                                            (isCompleted ? "DONE" : "IN_PROCESS");
	}
    
    private:
        double left;
        double right;
        char operation_sign;
};

class AddTask: public Task, public Named {
	AddTask(std::string const& name, Container<Task*>& container, Task* const task): 
                    Named(name), container(container), task(task) {}

	void run() override {
		if (isCompleted) 
            throw "The task was completed before";

		container.addLast(task);
		isCompleted = true;
	}

	std::string toString() const override{
		return name + ": " + "Add " + task->toString() + " to container ({})" +
											(isCompleted ? "DONE" : "IN_PROCESS");
	}
    private:
    	Container<Task*>& container;
	    Task* const task;
};

class CountObjectsTask: public Task, public Named {
	public:
		CountObjectsTask(std::string const& name, Container<Object*>& container): Named(name), container(container) {}

		void run() override{
			if (isCompleted) throw "The task was copleted before";
			result = container.getLen();
		}

		std::string toString() const override{
			return name + ": " + "Container has " + (isCompleted ? std::to_string(result) : "UNKNOWN") +  
										"objects (" + (isCompleted ? "DONE" : "IN_PROCESS") + ')';
		}
	private:
		Container<Object*>& container;
};

class CountTaskTask: public Task, public Named {
	public:
		CountTaskTask(std::string const& name, Container<Task*>& container): Named(name), container(container) {}

		void run() override {
			for (auto it = container.begin(); it != container.end(); it++) {
				Task* task = *it;
				if (task->hasResult())
					result++; 
			}
			isCompleted = true;
		}

		std::string toString() const override {
			return name + ": " + "Container has " + (isCompleted ? std::to_string(int(result)) : "UNKNOWN") +  
											" result task (" + (isCompleted ? "DONE" : "IN_PROCESS") + ')';
		}
	private:
		Container<Task*>& container;
};

class ClearTask: public Task {
	public:
		Container<Object*>& container;

		ClearTask(Container<Object*>& container): container(container) {}

		void run() override { 
			container.clear(); 
		}
};

class GetNumberOfObjects: public Task, public Named {
	public:
		GetNumberOfObjects(std::string const& name): Named(name) {}

		void run() override {
			result = Object::numEntities;
			isCompleted = true;
		}

		std::string toString() const override{
			return name + ": " + "Number of objects: " +
								(isCompleted ? std::to_string(int(result)) : "UNKNOWN") +
								" (" + (isCompleted ? "DONE" : "IN_PROCESS") + ')';
		}
};
