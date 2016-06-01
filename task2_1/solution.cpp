#include <iostream>
#include <regex>

using namespace std;
int main()
{
  string test1 = "2*x^100+100*x^2";
  regex re(
    R"_(([+-]){0,1}(\d*)\*(?:(x)(?:\^(\d*)){0,1}){0,1})_"
  );

  auto rstart = sregex_iterator(test1.begin(), test1.end(), re);
  auto rend = sregex_iterator();
  for (auto i = rstart; i != rend; i++)
  {
    smatch m = *i;
    string m_str = m.str();
    cout << m_str << endl;
  }
  return 0;
}
