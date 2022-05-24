# OOP-text-based-web-browser
In this lab, you will implement part of a text-based Web browser.  The program works by reading in an HTML file, constructing a collection of objects to represent the contents of that HTML file, and calling a virtual display() function on all of these objects to print out the web page and identify all of the hyperlinks.

The objects that represent the HTML contents form a hierarchy.  The base class, Contents, splits into two main types of content, Text and Tags.   Text (and its subclass Symbol) represent text to be displayed by the browser with no special modification, while Tags display their contents with some sort of formatting. 

Tags in HTML have several parts.  Tags generally start with an open tag (in the format <\p>), have some contents, then end with a closing tag (<\/p>).  Some tags do not have contents and may be denoted with a self-closing tag (<\p/>).  (In older HTML, some tags, like <\br>, were always considered to be self-closing, but newer standards recommend using self-closing tags like <\br/>.)  Tags may also specify attributes in the open tag, such as <\p id="myparagraph" style="color:red">.  The provided HTML parser will (or should) identify open, close, and self-closing tags, their contents, and attributes (if any), and store the type, contents, and attributes in the Tag data members string tag, vector<Contents*> contents, and unordered_map<string, string> attributes, respectively.

The browser recognizes 9 subclasses of Tag, described below:

-InvisibleTag:  a tag that doesn't display anything; e.g., a script<br>
-LineBreak:  tag representing a new line.  Displays a newline character (and its contents, if any)<br>
-Paragraph:  tag representing a paragraph.  Displays a newline ('\n') before and after its contents<br>
-Image:  tag that displays text based on its "alt" attribute, or "(Image)" if the alt attribute is not defined.  Any contents of the tag should appear afterwards.<br>
-UnorderedList:  tag representing a bulleted list.  Starts by outputting a newline ('\n'), then iterates through its contents, displaying the string " * " in front and a '\n' after everything it contains<br>
-OrderedList.  tag representing a numbered list.  Starts by outputting a newline ('\n'), then iterates through its contents, displaying "#. " before and '\n' after everything it contains, where # should 1 for the first element of contents, 2 for the second, etc. ("1. ", "2. ", "3. ", etc.)<br>
-Table, TableRow, TableColumn:  tags representing an entire table, a row of a table, and an entry in a table row, respectively.  Displays '\n' before the table, '\n' after each row, and '\t' after each entry.<br>

 I have provided code that implements much of the logic for this web browser, including functions that read in an HTML file and break it down into text, symbols, and tags; implementations for Contents, Text, Symbol, InvisibleTag, LineBreak, Table, TableRow, and TableColumn; a partial implementation for Tag; and a driver file.<br><br>

You will need to implement the following 6 functions to complete the program:<br><br>

-Tag* Tag::makeTag(const string& tag, const unordered_map<string, string>& attr, const vector<Contents*>& contents):  the "heart" of the program.  Takes in the Tag type, attributes, and contents, and it outputs a pointer to the appropriate subclass of Tag.  The correct subclass can be determined by the tag argument and the following table:<br><br>
                                  Argument value	Tag subclass<br>
                                  "style", "script", or "head"	InvisibleTag<br>
                                  "br" or "hr"	LineBreak<br>
                               "p", "h1", "h2", "h3", "h4", "h5", "h6", or "div"	Paragraph<br>
                                  "img"	Image<br>
                                  "ul"	UnorderedList<br>
                                  "ol"	OrderedList<br>
                                  "table"	Table<br>
                                  "tr"	TableRow<br>
                                  "td" or "th"	TableColumn<br>
                                  Anything else	Tag<br>
  <br><br>
All subclasses have a constructor that takes the 3 arguments of makeTag (tag, attr, and contents) as arguments.
<br><br>
-string Tag::getLinkTarget() const:  returns the value of the "href" attribute, or "" if there is no "href" attribute defined.  Tag attributes are stored in the data member unordered_map<string, string> attributes.<br>
-display() functions for Paragraph, Image, UnorderedList, and OrderedList.  <br>

