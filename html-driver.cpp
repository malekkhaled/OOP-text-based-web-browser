#include <iostream>
#include <fstream>
using namespace std;

#include "html.h"

int main()
{
  Webpage page;
  page.loadFile("input.html");
  page.display(cout);
  vector<const HTML::Tag*> links = page.getLinks();

  cout << "There are " << links.size() << " links on this page:\n";

  auto dummy = links; dummy.clear();
  for (const HTML::Tag* l : links)
  {
    l->display(cout, dummy);
    cout << " (" << l->getLinkTarget() << ")\n";
  }

  return 0;
}