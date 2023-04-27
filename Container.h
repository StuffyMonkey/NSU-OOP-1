#pragma once
#include <initializer_list>
#include <iterator>

template <typename T>
class Container {
    private:
        // base class for storing data in container
        class Cell {
            public:
            T data;
            Cell* next;
            Cell* prev;

            Cell(T data) {
                this->data = data;
                this->next = nullptr;
                this->prev = nullptr;
            }
        };

        template <typename K>
        class Iterator {
            public:
                Iterator(Cell* init) : ptr(init){}

                // Prefix increment
                Iterator& operator++() {
                    ptr = ptr->next;
                    return *this;
                }

                Iterator& operator--() {
                    ptr = ptr->prev;
                    return *this;
                }

                // Postfix increment
                Iterator operator++(int) {
                    Iterator tmp = *this;
                    ++(*this);
                    return tmp;
                }

                Iterator operator--(int) {
                    Iterator tmp = *this;
                    --(*this);
                    return tmp;
                }

                bool operator==(const Iterator& other) {
                    return ptr == other.ptr;
                };

                bool operator!=(const Iterator& other) {
                    return ptr != other.ptr;
                };

                T& operator*() const {
                    return ptr->data;
                }

                T* operator->() const {
                    return &ptr->data;
                }

            private:
                Cell* ptr;
        };

    public:
        Container(const std::initializer_list<T>& iterable) {
            for(auto it = iterable.begin(); it != iterable.end(); it++) {
                addLast(*it);
            }
        }

        Container() = default;

        void addLast(T data) {
            Cell* newCell = new Cell(data);
            len++;

            if (tail == nullptr) {
                tail = newCell;
                head = tail;
                return;
            }
            else {
                Cell* tmp = tail;
                tail = newCell;
                tail->prev = tmp;
                tmp->next = tail;
                return;
            }

        }

        void addFirst(T data) {
            Cell* newCell = new Cell(data);
            len++;

            if (head == nullptr) {
                head = newCell;
                tail = head;
                return;
            }
            else {
                Cell* tmp = head;
                head = newCell;
                head->next = tmp;
                tmp->prev = head;
                return;
            }
        }

        Iterator<T> begin() {
            return Iterator<T>(head);
        }

        Iterator<T> end() {
            return Iterator<T>(nullptr);
        }

        void removeFirst() {
            Cell* tmp = head;
            if (! isEmpty()) {
                head = tmp->next;
                if (head != nullptr)
                    head->prev = nullptr;
                else
                    tail = nullptr;
                free(tmp);
                len--;
            }
            else
                throw std::out_of_range("Attempt to remove element from empty container");
        }

        T getFirst() {
            if (! isEmpty())
                return head->data;
            else
                throw std::out_of_range("Attempt to get element from empty container");
        }

        T getLast() {
            if (! isEmpty())
                return tail->data;
            else
                throw std::out_of_range("Attempt to get element from empty container");
        }

        

        bool isEmpty() {
            return (len == 0);
        }

        int getLen() {
            return len;
        }

        void removeLast() {
            Cell* tmp = tail;
            if (! isEmpty()) {
                tail = tmp->prev;
                if (tail != nullptr)
                    tail->next = nullptr;
                else
                    head = nullptr;
                free(tmp);
                len--;
            }
             else
                throw std::out_of_range("Attempt to remove element from empty container");
        }

        void reverseContainer() {
            Cell* tmp = head;
            while (tmp != nullptr) {
                Cell* swap = tmp->next;
                tmp->next = tmp->prev;
                tmp->prev = swap;
                tmp = swap;
            }
            tmp = head;
            head = tail;
            tail = tmp;
        }

        void freeContainer() {
            Cell* current = head;
            while (! isEmpty()) {
                removeFirst();
            }
        }


        ~Container() {
            freeContainer();
        }

        // manual cleaning
        void clear() { 
            freeContainer(); 
        }

    private:
        int len = 0;
        Cell* head = nullptr;
        Cell* tail = nullptr;
};

