#include <vector>
#include <string>

class Date {
public:
    Date(int m, int d): month(m), day(d) {}
    int daysInMonth() {
        std::vector<int> days {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        return days[month - 1];
    }
    int getDay() { return day; }
    int getMonth() { return month; }
    void nextDay() {
        ++day;
        if (daysInMonth() < day) {
            ++month;
            day = 1;
        }
        if (month == 13) {
            month = 1;
        }
    }
    std::string toString() {
        std::string m = std::to_string(month), d = std::to_string(day);
        if (month < 10) {
            m = "0" + m;
        }
        if (day < 10) {
            d = "0" + d;
        }
        return m + "/" + d;
    }

private:
    int month, day;

};