#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

class GPACalculator {
public:
  using CreditHours = int;
  enum Grade { A = 5, B = 4, C = 3, D = 2, BAD_GRADE = 0};
  using Record = std::tuple<std::string, CreditHours, Grade>;
  using Records = std::vector<Record>;

  float calc_gpa() {
    if (_records.empty()) {
      return 0.0;
    }
    int dividend = 0;
    int divisor = 0;
    for (const auto& rec : _records) {
      dividend += std::get<1>(rec) * static_cast<int>(std::get<2>(rec));
      divisor += std::get<1>(rec);
    }
    float ret = (((float)dividend / divisor)/5)*4;
    return ret;
  }

  void add_record(const std::string& course, CreditHours credit,
                  Grade grade) {
    _records.push_back(std::make_tuple(course, credit, grade));
  }

  Grade getGrade(const std::string& gradeStr) {
    if (!gradeStr.compare("A") || !gradeStr.compare("a")) {
      return A;
    }
    if (!gradeStr.compare("B") || !gradeStr.compare("b")) {
      return B;
    }
    if (!gradeStr.compare("C") || !gradeStr.compare("c")) {
      return C;
    }
    if (!gradeStr.compare("D") || !gradeStr.compare("d")) {
      return D;
    }
    return BAD_GRADE;
  }

  void print_stats() {
    std::cout << "Number of courses considered: " << _records.size() << std::endl;
  }
  void add_records(const std::string& fileName);
private:
  Records _records;
};


void GPACalculator::add_records(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.good()) {
    std::cerr << "Could not open file\n";
    return;
  }
  std::string line;
  while(std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string course, gradeStr;
    int hours;
    if(!(iss >> course >> hours >> gradeStr)) {
      std::cerr << "Invalid line in the file. " <<
        line <<
        "Please enter in the following format\n";
      std::cerr << "<Course Name> <Credit Hours> <Grade>\n";
      _records.clear();
      return;
    }
    Grade grade = getGrade(gradeStr);
    if (BAD_GRADE == grade) {
      std::cerr << "Invalid grade : "
                << line << " . Grade can only be A, B, C or D\n";
      _records.clear();
      return;
    }
    add_record(course, hours, grade);
  }
}

int main() {

  GPACalculator gpaCalculator;
  std::string fileName;
  std::cout << "Enter the file name containing grades: ";
  std::cin >> fileName;

  gpaCalculator.add_records(fileName);
  float gpa = gpaCalculator.calc_gpa();

  std::cout << "The GPA is: " << gpa << std::endl;
  gpaCalculator.print_stats();
  return 0;
}
