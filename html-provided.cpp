#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
# include <memory>
using namespace std;

#include "html.h"
using namespace HTML;

unordered_map<string, string> Symbol::lookup {
   {"gt", ">"},
   {"lt", "<"},
   {"quot", "\""},
   {"amp", "&"},
   {"nbsp", " "},
   {"mdash", "--"},
   {"ndash", "-"},
   {"times", "X"},
   {"raquo", ">>"},
   {"laquo", "<<"}};

string Contents::parseHTML(istream& in, vector<Contents*>& contents)
{
  char ch = in.get();
  while (in)
  {
    if (ch == '<')
    {
      ch = in.get();
      if (ch == '/')
      {
        string close;
        getline(in, close, '>');
        return close;
      }
      else if (ch == '!') //<!DOCTYPE or <!--
      {
        ch = in.get();
        if (ch == '-')
        {
          in.ignore(); //-
          string end("-->");
          int i = 0;
          do
          {
            ch = in.get();
            if (ch == end[i])
              i++;
            else
              i = 0;
          } while (in && i < end.size());
        }
        else
        {
          string ignore;
          getline(in, ignore, '>');
        }
      }
      else
      {
        in.unget();
        string close = Tag::parseTag(in, contents);
        if (close != "")
          return close;
      }
    }
    else if (ch == '&')
      contents.push_back(new Symbol(in));
    else //if (!isspace(ch))
    {
      in.unget();
      Text* text = new Text(in);
      if (text->getText() != " " || (!contents.empty() && contents.back()->getTag() == "a"))
        contents.push_back(text);
      else
        delete text;
    }
    ch = in.get();
  }
  return "";
}

string Tag::parseTag(istream& in, vector<Contents*>& contents)
{
  ostringstream oss;
  unordered_map<string, string> attr;
  vector<Contents*> innerContents;
  char ch;
  string close;

  ch = in.get();
  while (in && !isspace(ch) && ch != '>' && ch != '/')
  {
    oss << (char) tolower(ch);
    ch = in.get();
  }
  if (isspace(ch))
  {
    parseAttributes(in, attr);
    ch = in.get();
  }
  if (ch == '/')
    in.ignore(); //>
  else if (oss.str() != "script")
    close = Contents::parseHTML(in, innerContents);
  else
  {
    string end = "</script>";
    int i = 0;
    do
    {
      ch = in.get();
      if (ch == end[i])
        i++;
      else
        i = 0;
    } while (in && i < end.size());
  }
  
  contents.push_back(makeTag(oss.str(), attr, innerContents));
  if (close == oss.str() || close == "")
    return "";
  else
  {
    //cout << "Malformed tag " << oss.str() << ":  closed by " << close << '\n';
    return close;
  }
}

void Tag::parseAttributes(istream& in, unordered_map<string, string>& attr)
{
  string key, value;
  char ch;
  do
  {
    ch = in.get();
  } while (in && isspace(ch));
  
  while (in && ch != '/' && ch != '>')
  {
    in.unget();
    getline(in, key, '=');
    char delim = in.get();
    if (delim != '"' && delim != '\'')
    {
      //cout << "Error:  bad attribute value delimiter (" << delim << ")\n";
      getline(in, key, '>');
      return;
    }
    getline(in, value, delim);
    //cout << "Attribute " << key << " = \"" << value << "\"\n";
    attr[key] = value;
    ch = in.get();
    while (in && isspace(ch))
      ch = in.get();
  }
  in.unget();
}

Text::Text(istream& in)
{
  ostringstream oss;
  char ch;

  ch = in.get();
  while (in && ch != '<' && ch != '&')
  {
    if (isspace(ch))
    {
      oss << ' ';
      do
      {
        ch = in.get();
      } while (in && isspace(ch));
    }
    else
    {
      oss << ch;
      ch = in.get();
    }
  }
  in.unget();

  text = oss.str();
}

Symbol::Symbol(istream& in)
{
  text = '?';

  char temp = in.get();
  //if (isdigit(temp))
  //{
  //  in.unget();
  //  temp = '#';
  //}
  if (temp == '#')
  {
    int i;
    temp = in.get();
    if (temp == 'x')
      in >> hex >> i;
    else
    {
      in.unget();
      in >> i;
    }
    in.ignore(); //;
    text = (char) i;
  }
  else
  {
    ostringstream oss;
    while (in && temp != ';')
    {
      oss << temp;
      temp = in.get();
    }

    if (lookup.count(oss.str()) > 0)
      text = lookup[oss.str()];
    else
      cout << "Unrecognized symbol:  <" << oss.str() << ">\n";
  }
}

Tag::Tag(const string& tag, const unordered_map<string, string>& attr, const vector<Contents*>& contents)
 : tag(tag), attributes(attr), contents(contents)
{}

Tag::Tag(const Tag& copy)
{
  throw runtime_error("Copying tags is not supported");
}

Tag& Tag::operator=(const Tag& rhs)
{
  throw runtime_error("Copying tags is not supported");
  return *this;
}

Tag::~Tag()
{
  for (Contents* c : contents)
    delete c;
}

void Text::display(ostream& out, vector<const Tag*>& links) const
{
  out << text;
}

void Tag::display(ostream& out, vector<const Tag*>& links) const
{
  if (getLinkTarget() != "")
    links.push_back(this);
  for (Contents* c : contents)
    c->display(out, links);
}

void InvisibleTag::display(ostream& out, vector<const Tag*>& links) const
{}

void LineBreak::display(ostream& out, vector<const Tag*>& links) const
{
  out << '\n';
  Tag::display(out, links);
}

void Table::display(ostream& out, vector<const Tag*>& links) const
{
  out << '\n';
  Tag::display(out, links);
}

void TableRow::display(ostream& out, vector<const Tag*>& links) const
{
  Tag::display(out, links);
  out << '\n';
}

void TableColumn::display(ostream& out, vector<const Tag*>& links) const
{
  Tag::display(out, links);
  out << '\t';
}

void Webpage::loadFile(const string& file)
{
  ifstream in(file);
  vector<Contents*> vec;
  Contents::parseHTML(in, vec);
  contents = vec[0];
}

void Webpage::display(ostream& out)
{
  links.clear();
  if (contents != nullptr)
    contents->display(out, links);
}

