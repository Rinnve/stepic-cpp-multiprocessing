#include <iostream>
#include <regex>
#include <set>
#include <sstream>

struct Element 
{
  int coeff, power;
  Element(int c, int p): coeff(c), power(p) {}
};

struct ElementComparator 
{
  bool operator()(const Element &a, const Element &b) const
  {
    return a.power > b.power; 
  }
};

std::string derivative(std::string polynomial) {
  using namespace std;
    regex re(
    R"_(([+-]){0,1}(\d*)(?:\*){0,1}(?:(x)(?:\^(\d*)){0,1}){0,1})_"
  );

  set<Element, ElementComparator> elements;

  auto rstart = sregex_iterator(polynomial.begin(), polynomial.end(), re);
  auto rend = sregex_iterator();
  for (auto i = rstart; i != rend; i++)
  {
    smatch m = *i;
    string m_str = m.str();
    if (m_str == "") continue;
    /*
    cout << "Full match: " << m_str << endl;
    cout << "1: " << m[1] << endl;
    cout << "2: " << m[2] << endl;
    cout << "3: " << m[3] << endl;
    cout << "4: " << m[4] << endl;
    */
    int sign = (m[1] == "+" || m[1] == "") ? 1 : -1;
    int coeff = (m[2] == "") ? 1 : stoi(m[2]);
    coeff *= sign;
    //cout << "Coefficient: " << coeff << endl;
    int power;
    if (m[4] == "" || m[4] == "0")
    {
      if (m[3] == "x") power = 1;
      else power = 0;
    }
    else power = stoi(m[4]);
    //cout << "Power: " << power << endl;
    //cout << "----" << endl;
    Element elem = Element(coeff, power);
    auto old_elem_i = elements.find(elem);
    if (old_elem_i != elements.end())
    {
      elem.coeff += (*old_elem_i).coeff;
      elements.erase(old_elem_i);
    }
    elements.insert(elem);
  }
  stringstream output;
  bool first = true;
  for (auto i = elements.begin(); i != elements.end(); i++)
  {
    int new_coeff = (*i).coeff * (*i).power;
    int new_power = (*i).power - 1;
    if (new_power >= 0)
    {
      if (new_coeff > 0 && !first)
      {
        output << "+";        
      } 
      if (new_power > 0)
      {
        if (abs(new_coeff) != 1) output << new_coeff << "*x";
        if (new_power > 1) output << "^" << new_power;
      } 
      else output << new_coeff;
    }
    first = false;
  }
  return output.str();
}


int main()
{
  std::string input;
  std::cin >> input;
  std::string result = derivative(input);
  std::cout << result << std::endl; 
  return 0;
}
