#include <iostream>
#include <regex>

using namespace std;
int main()
{
  string test1 = "2*x^100-100*x^2+5";
  regex re(
    R"_(([+-]){0,1}(\d*)(?:\*(x)(?:\^(\d*)){0,1}){0,1})_"
  );

  auto rstart = sregex_iterator(test1.begin(), test1.end(), re);
  auto rend = sregex_iterator();
  for (auto i = rstart; i != rend; i++)
  {
    smatch m = *i;
    string m_str = m.str();
    if (m_str == "") continue;
    cout << "Full match: " << m_str << endl;
    int sign = (m[1] == "+" || m[1] == "") ? 1 : -1;
    int coeff = (m[2] == "") ? 1 : stoi(m[2]);
    coeff *= sign;
    cout << "Coefficient: " << coeff << endl;
    int power = (m[4] == "" || m[4] == "0") ? 0 : stoi(m[4]);
    cout << "Power: " << power << endl;
    cout << "----" << endl;
  }
  return 0;
}
