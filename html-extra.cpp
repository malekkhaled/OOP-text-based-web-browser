#include "html.h"
using namespace HTML;

void Text::writeHTML(ostream& out) const
{
  out << text;
}

void Symbol::writeHTML(ostream& out) const
{
  if (text.size() > 0)
    out << "&#" << (int) text[0] << ';';
}

void Tag::writeHTML(ostream& out) const
{
  out << '<' << tag;
  if (!attributes.empty())
    for (auto entry : attributes)
      out << ' ' << entry.first << "=\"" << entry.second << '"';
  if (contents.empty())
    out << "/>\n";
  else
  {
    out << ">\n";
    for (auto c : contents)
      c->writeHTML(out);
    out << "</" << tag << ">\n";
  }
}

string Tag::getID() const
{
  if (attributes.count("id") > 0)
    return attributes.at("id");
  else
    return "";
}

Tag* Tag::getChildByTag(const string& tag, unsigned count)
{
  for (Contents* ptr : contents)
    if (ptr->getTag() == tag)
      if (count == 0)
        return reinterpret_cast<Tag*>(ptr);
      else
        count--;
  return nullptr;
}

Tag* Tag::getChildByID(const string& id)
{
  for (Contents* ptr : contents)
    if (ptr->getID() == id)
      return reinterpret_cast<Tag*>(ptr);
  return nullptr;
}

Tag* Tag::searchForID(const string& id)
{
  if (getID() == id)
    return this;

  Tag* ret = nullptr;
  for (Contents* ptr : contents)
    if (ret = ptr->searchForID(id)) //searchForID != nullptr
      return ret;
  return nullptr;
}
