#include<fstream>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#include<cmath>

double countDispersion (std::vector <int> &vec, double avg, int quantity, int start, int finish)
{
	double dispSum = 0;
	for (int i = start ; i < finish; i ++)
	{
		dispSum +=pow (vec[i] - avg, 2);
	}
	return (double) dispSum / quantity;
}

int main()
{
	//take data from input.txt file
	std::ifstream in("merged1.txt");
	std::ofstream out("output.txt");
	// write answer in output.txt file


    std::string query, url, line, scoreString;
    std::map <std::pair <std::string, std::string>, std::pair <std::vector <int> , std::pair <int, int> > >m;
	int score, firstTab, secondTab;


	// take an input.txt with data, each line shold have format : query<Tab>url<Tab>score.
	//Score goes right after secondTab without spaces
	while (getline(in, line))
	{
		firstTab = -1;
		secondTab = -1;
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == '\t')
			{
				if (firstTab == -1) {
					firstTab = i;
				} else {
					secondTab = i;
					break;
				}
			}
		}

		query = line.substr(0, firstTab);
		url = line.substr(firstTab + 1, secondTab - firstTab - 1);
		scoreString = line.substr(secondTab + 1, 1);
		score = scoreString[0] - '0';
		m[{query, url}].first.push_back(score);
		m[{query, url}].second.first += score;
		m[{query, url}].second.second += 1;
	}
	// we got map calls m with structure: Key:<query, url>
	//Value:  <vector of scores for pair <query, url> , <total sum of scores, quantity of scores> >


	// we're going to get avgScore for each pair <query, url>, and correct it.
	// we will erase max and min element and recount avgScore while the resulting avgScore will not differ from the previous avgScore more than cff1.
	// cff1 is coefficient. Shows when we accept value of dispersion
	//cff2 is coefficient. If quantity of scores for this pair <query, url> is lower than cff2, then we think the avgScore is real rate
	// cff1 and cff2 are selected practically
	double cff1, avgScore, dispersion, dispersionPrevious, dispSum;
	int cff2, currSum, currQuantity, iteration, size;
    for (auto elemet : m)
    {
    	std::sort (elemet.second.first.begin(), elemet.second.first.end());
		currSum = elemet.second.second.first;
		currQuantity  = elemet.second.second.second;
		size = currQuantity;
		avgScore = (double) currSum / currQuantity;
        dispSum = 0;
		dispersion = countDispersion(elemet.second.first, avgScore, currQuantity, 0 , size);
        cff1 = 0.5;
        cff2 = 3;
        iteration = -1;
		if (currQuantity >= 3)
			do {
				iteration++;
				currSum -= elemet.second.first[iteration] + elemet.second.first[size - 1 - iteration];
				currQuantity -= 2;
				avgScore = (double) currSum / currQuantity;
				dispersionPrevious = dispersion;
				dispersion = countDispersion(elemet.second.first, avgScore, currQuantity, iteration + 1, size - 1 - iteration );
			} while (currQuantity >= 3 && dispersion > cff1);
			// we could also look on difference between dispersionPrevious dispersion
			//} while (currQuantity >= 3 && (fabs (dispersionPrevious - dispersion) > cff1));
		out << elemet.first.first << "\t" << elemet.first.second << "\t" <<  round(avgScore) << "\n";
		//uncomment line below and comment line higher to check how many iteration it makes for each pair
		//out << elemet.first.first << "\t" << elemet.first.second << "\t" << round(avgScore) << "\t" << iteration << "\n";
    }
	// write in output.txt the rate close to real for each pair <query, url> in format: "query<Tab>url<Tab>score\n"

    return 0;
}

