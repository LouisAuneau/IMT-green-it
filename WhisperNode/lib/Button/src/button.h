#ifndef BUTTON_H
#define BUTTON_H

class Button {
    public: 
        Button(int pin);
        void check();
        int getNumber();

    private:
        int m_pin;
        int m_counter;
        bool m_flag;
        long m_timer;
};

#endif