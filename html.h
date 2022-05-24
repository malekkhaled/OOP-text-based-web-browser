#ifndef __HTML_H
#define __HTML_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

namespace HTML
{
  class Tag;

  class Contents
  {
  public:
    static string parseHTML(istream&, vector<Contents*>&);
    
    Contents() {}
    virtual void display(ostream&, vector<const Tag*>&) const = 0;
    virtual void writeHTML(ostream&) const = 0;
    virtual string getTag() const { return ""; }
    virtual string getID() const { return ""; }
    virtual string getLinkTarget() const { return ""; }
    virtual Tag* searchForID(const string&) { return nullptr; }
    virtual Tag* searchForTag(const string&, int) { return nullptr; }
    virtual ~Contents() {}
  };

  class Text : public Contents
  {
  protected:
    string text;
  public:
    Text() {}
    Text(istream&);
    string getText() const { return text; }
    virtual void display(ostream& out, vector<const Tag*>&) const override;
    virtual void writeHTML(ostream& out) const override;
  };

  class Symbol : public Text
  {
  private:
    static unordered_map<string, string> lookup;
  public:
    Symbol(istream&);
    virtual void writeHTML(ostream& out) const override;
  };

  class Tag : public Contents
  {
  protected:
    string tag;
    unordered_map<string, string> attributes;
    vector<Contents*> contents;
  public:
    static string parseTag(istream&, vector<Contents*>&);
    static void parseAttributes(istream&, unordered_map<string, string>&);
    static Tag* makeTag(const string&, const unordered_map<string, string>& map, const vector<Contents*>& contents);

    Tag(const string& str, const unordered_map<string, string>& map, const vector<Contents*>& vec);
    virtual void display(ostream&, vector<const Tag*>&) const override;
    virtual void writeHTML(ostream&) const override;
    virtual string getLinkTarget() const override;

    Tag(const Tag&);
    Tag& operator=(const Tag&);
    virtual ~Tag();
    virtual string getID() const override;
    virtual string getTag() const override { return tag; }
    const vector<Contents*>& getChildren() const { return contents; }
    Tag* getChildByTag(const string&, unsigned count = 0);
    Tag* getChildByID(const string&);
    Tag* searchForTag(const string&);
    Tag* searchForID(const string&);
  };

  class InvisibleTag : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream&, vector<const Tag*>&) const override;
  };

  class LineBreak : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& vec) const override;
  };

  class Table : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class TableRow : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class TableColumn:  public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class Paragraph : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class UnorderedList : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class OrderedList : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream& out, vector<const Tag*>& links) const override;
  };

  class Image : public Tag
  {
  public:
    using Tag::Tag;
    virtual void display(ostream&, vector<const Tag*>& links) const override;
  };
}

class Webpage
{
private:
  HTML::Contents* contents;
  vector<const HTML::Tag*> links;
public:
  Webpage() { contents = nullptr; }
  void loadFile(const string&);
  void display(ostream&);
  void writeHTML(ostream& out) const
    { contents->writeHTML(out); }
  const vector<const HTML::Tag*>& getLinks() const 
    { return links; }
  ~Webpage() 
    { delete contents; }
};

#endif