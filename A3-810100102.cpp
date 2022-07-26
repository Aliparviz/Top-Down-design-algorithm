//بسم الله الرحمن الرحیم
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <set>
#define Donoghte ":"
#define Space  " "
#define First 0
#define Day_by_minutes 1440
#define Initial_value 0
#define No_translator "Not Found"
using namespace std;
struct Time
{
	int hours;
	int minutes;
};
struct Language_Options
{
	string language;
	string translator;
	int counter = 0;
};
struct Translator
{
	string name;
	Time start_free_time;
	Time End_free_time;
	vector<bool>overall_time;
	vector<string>languages;
	int language_counter = 0;
};
struct Event
{
	string name;
	Time start_time;
	Time end_time;
	vector<bool>overall_time;
	vector<Language_Options> languages;
};
bool sort_numbers(Language_Options one, Language_Options two)
{
	return(one.counter<two.counter);
}
vector<string>split_by_spaces(string line)
{
	vector<string>result;
	int start_pos = 0;
	while (start_pos < line.length())
	{
		int comma_pos = line.find(Space, start_pos);
		if (comma_pos == string::npos)
		{
			result.push_back(line.substr(start_pos));
			break;
		}
		result.push_back(line.substr(start_pos, comma_pos - start_pos));
		start_pos = comma_pos + 1;
	}
	return result;
}
vector<string>tokenize(string time_str, string donoghte)
{
	int start = 0;
	int end = time_str.find(donoghte);
	vector<string>temporary;
	while (end != -1)
	{
		temporary.push_back(time_str.substr(start, end - start));
		start = end + donoghte.size();
		end = time_str.find(donoghte, start);
	}
	temporary.push_back(time_str.substr(start, end - start));
	return temporary;
}
Time convert_time(string time_str)
{
	Time result;
	string donoghte = Donoghte;
	vector<string>str = tokenize(time_str, donoghte);
	result.hours = stoi(str[0]);
	result.minutes = stoi(str[1]);
	return result;
}
void initial_event_ovtime(Event& event)
{
	for (int i = 0; i < Day_by_minutes; i++)
	{
		event.overall_time.push_back(false);
	}

	for (int i = ((event.start_time.hours * 60) + event.start_time.minutes)
		; i < ((event.end_time.hours * 60) + event.end_time.minutes); i++)
	{
		event.overall_time[i] = true;
	}
}
void initial_tranlator_ovtime(Translator& translator)
{
	for (int i = 0; i < Day_by_minutes; i++)
	{
		translator.overall_time.push_back(false);
	}

	for (int i = ((translator.start_free_time.hours * 60) + translator.start_free_time.minutes)
		; i < ((translator.End_free_time.hours * 60) + translator.End_free_time.minutes); i++)
	{
		translator.overall_time[i] = true;
	}
}
Translator read_translator(string line)
{
	Translator translator;
	vector <string>fields = split_by_spaces(line);
	translator.name = fields[0];
	translator.start_free_time = convert_time(fields[1]);
	translator.End_free_time = convert_time(fields[2]);

	for (int i = 3; i < fields.size(); i++)
	{
		translator.languages.push_back(fields[i]);
	}
	translator.language_counter = translator.languages.size();
	initial_tranlator_ovtime(translator);
	return translator;
}
Event read_events(string line)
{
	Event event;
	vector<string> fields = split_by_spaces(line);

	Language_Options obj;

	event.name = fields[0];
	event.start_time = convert_time(fields[1]);
	event.end_time = convert_time(fields[2]);
	for (int i = 3; i < fields.size(); i++)
	{
		obj.language = fields[i];
		event.languages.push_back(obj);
	}
	initial_event_ovtime(event);

	return event;
}
void cal_number_of_each_language(vector<Translator>translators_list, Event& event)
{
	{
		for (int i = 0; i < translators_list.size(); i++)
		{
			for (int j = 0; j <translators_list[i].languages.size(); j++)
			{
				for (int k = 0; k < event.languages.size(); k++)
				{
					if (event.languages[k].language == translators_list[i].languages[j])
					{
						event.languages[k].counter++;
					}
				}
			}
		}
	}
}
void sort_languages(Event& event)
{
	sort(event.languages.begin(), event.languages.end(), sort_numbers);
}
void sort_tranlators(vector<Translator>&translators_list)
{
	for (int i = 0; i < translators_list.size(); i++)
	{
		for (int j = 0; j<translators_list.size(); j++)
		{
			if (translators_list[i].language_counter < translators_list[j].language_counter)
			{
				Translator flag = translators_list[i];
				translators_list[i] = translators_list[j];
				translators_list[j] = flag;
			}
			else if (translators_list[i].language_counter == translators_list[j].language_counter)
			{
				if (translators_list[i].name < translators_list[j].name)
				{
					Translator flag = translators_list[i];
					translators_list[i] = translators_list[j];
					translators_list[j] = flag;
				}
			}
		}
	}

}
void make_translator_time_false(Translator& translator, Event event)
{
	for (int i = (event.start_time.hours) * 60 + event.start_time.minutes;
		i < (event.end_time.hours) * 60 + event.end_time.minutes; i++)
	{
		translator.overall_time[i] = false;
	}
}
int check_time(Translator translator, Event event)
{
	for (int i = (event.start_time.hours) * 60 + event.start_time.minutes;
		i < (event.end_time.hours) * 60 + event.end_time.minutes; i++)
	{
		if (translator.overall_time[i] == false)
		{
			return 0;
		}
	}
	return 1;
}
void choose_translator(vector<Translator>&translators_list, Event& event)
{
	for (int i = 0; i < event.languages.size(); i++)
	{
		for (int j = 0; j < translators_list.size(); j++)
		{
			for (int k = 0; k < translators_list[j].languages.size(); k++)
			{
				if ((event.languages[i].language == translators_list[j].languages[k]) &&
					(check_time(translators_list[j], event) == 1))
				{
					event.languages[i].translator = translators_list[j].name;
					make_translator_time_false(translators_list[j], event);
					i++;
					if (i >= event.languages.size())
					{
						return;
					}
					break;
				}
			}
		}
	}
}
void print_result(vector<Event>events_list, vector<Event>events_list_copy)
{
	for (int i = 0; i < events_list.size(); i++)
	{
		for (int j = 0; j < events_list[i].languages.size(); j++)
		{
			for (int k = 0; k < events_list[i].languages.size(); k++)
			{
				if (events_list[i].languages[k].language == events_list_copy[i].languages[j].language)
				{
					events_list_copy[i].languages[k].translator = events_list[i].languages[j].translator;
				}
			}
		}
	}
	for (int i = 0; i < events_list_copy.size(); i++)
	{
		cout << events_list_copy[i].name << endl;
		for (int j = 0; j < events_list_copy[i].languages.size(); j++)
		{
			if (events_list_copy[i].languages[j].translator == "")
			{
				events_list_copy[i].languages[j].translator = No_translator;
			}
			cout << events_list_copy[i].languages[j].language << ": " << events_list_copy[i].languages[j].translator << endl;
		}
	}
}
void manage_event(vector<Event>&events_list, vector<Translator>&translators_list)
{
	for (int i = 0; i < events_list.size(); i++)
	{
		cal_number_of_each_language(translators_list, events_list[i]);
		sort_languages(events_list[i]);
		sort_tranlators(translators_list);
		choose_translator(translators_list, events_list[i]);
	}
}
void read_file(vector<Event>&events_list, vector<Translator>&translators_list,char* path)
{
	ifstream inpt;
	string line;

	inpt.open(path);
	getline(inpt, line);
	int Tnumber = stoi(line);
	for (int i = 0; i < Tnumber; i++)
	{
		getline(inpt, line);
		translators_list.push_back(read_translator(line));
	}
	getline(inpt, line);
	int Enumber = stoi(line);
	for (int i = 0; i < Enumber; i++)
	{
		getline(inpt, line);
		events_list.push_back(read_events(line));
	}
	inpt.close();
}
int main(int argc, char *argv[])
{
	vector<Event>events_list;
	vector<Translator>translators_list;
	read_file(events_list,translators_list,argv[1]);
	vector<Event>events_list_copy = events_list;
	manage_event(events_list, translators_list);
	print_result(events_list, events_list_copy);
	return 0;
}
