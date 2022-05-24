#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
# include <memory>
using namespace std;

#include "html.h"
using namespace HTML;


Tag* Tag :: makeTag(const string& tag, const unordered_map <string,string>& attr, const vector <Contents*>& contents)
{
  if (tag == "style" || tag == "script" || tag == "head")
    return new InvisibleTag (tag, attr, contents);

  else if (tag == "br" || tag == "hr")
    return new LineBreak(tag, attr, contents);

  else if (tag == "p" || tag == "h1" || tag == "h2" || tag == "h3" || tag == "h4" || tag == "h5" || tag == "h6" || tag == "div")
    return new Paragraph(tag, attr, contents);

  else if (tag == "img")
    return new Image(tag, attr, contents);

  else if (tag == "ul")
    return new UnorderedList(tag, attr, contents);
  
  else if (tag == "ol")
    return new OrderedList(tag, attr, contents);

  else if (tag == "table")
    return new Table(tag, attr, contents);
  
  else if(tag == "tr")
    return new TableRow(tag, attr, contents);

  else if (tag == "td" || tag == "th")
    return new TableColumn(tag, attr, contents);
  
  else
    return new Tag(tag, attr, contents);
  
}

string Tag :: getLinkTarget() const
{
  string ret;
  if (attributes.find("href") != attributes.end())
    ret = attributes.at("href");
  else 
    ret = "";
  
  return ret;
}

void Paragraph::display(ostream& out, vector<const Tag*>& links) const
{
  out << '\n';
  Tag::display(out, links);
  out << '\n';
}

void Image::display(ostream& out, vector<const Tag*>& links) const
{
    if (attributes.find("alt") != attributes.end())
        out << attributes.at("alt");
    else 
        out << "(Image)";

    for (int i = 0; i < contents.size(); i++)
    {
        contents[i] -> display(out,links);
    }
}

void UnorderedList::display(ostream& out, vector<const Tag*>& links) const
{
  out << '\n';
  if (getLinkTarget() != "")
    links.push_back(this);
  for (Contents* c : contents)
  {
    out << " * ";
    c->display(out, links);
    out << '\n';
  }

}

void OrderedList::display(ostream& out, vector<const Tag*>& links) const
{
  int i = 1;
  out << '\n';
  if (getLinkTarget() != "")
    links.push_back(this);
  for (Contents* c : contents)
  {
    out << i << ". ";
    c->display(out, links);
    out << '\n';
    i++;
  }
}