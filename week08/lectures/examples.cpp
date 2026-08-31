#include <iostream>
#include <stdexcept>
#include <vector>

void SwapByValue(int left, int right) {
  int temporary = left;
  left = right;
  right = temporary;
}

void SwapByPointer(int* left, int* right) {
  int temporary = *left;
  *left = *right;
  *right = temporary;
}

void SwapByReference(int& left, int& right) {
  int temporary = left;
  left = right;
  right = temporary;
}

void PrintPair(const char* label, int left, int right) {
  std::cout << label << ": (" << left << ", " << right << ")\n";
}

void Normalize(std::vector<double>& values) {
  double sum = 0.0;
  for (double value : values) {
    sum += value;
  }
  if (sum == 0.0) {
    throw std::invalid_argument{"cannot normalize a zero sum"};
  }
  for (double& value : values) {
    value /= sum;
  }
}

int main() {
  int x = 5;
  int y = 7;
  PrintPair("initial", x, y);
  SwapByValue(x, y);
  PrintPair("by value", x, y);
  SwapByPointer(&x, &y);
  PrintPair("by pointer", x, y);
  SwapByReference(x, y);
  PrintPair("by reference", x, y);

  std::vector<double> values{1.0, 2.0, 3.0};
  Normalize(values);
  for (double value : values) {
    std::cout << value << ' ';
  }
  std::cout << '\n';
}
