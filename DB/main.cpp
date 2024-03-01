#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <cstdio>
#include <Windows.h>
#include <regex>
#include <iterator>

#include "stringman.hpp"

using namespace std;
namespace fs = std::filesystem;

void show(vector<string>& vec) {
	for (auto& str : vec) {
		cout << str << '\n';
	}
}

void show(unordered_map<string, int>& albums) {
	for (auto& pair : albums) {
		cout << pair.first << ", " << pair.second << '\n';
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	ifstream inFile;
	ofstream outFile;

	ofstream songFile;
	ofstream albumFile;
	ofstream genreFile;
	ofstream artistFile;
	ofstream songArtistFile;
	ofstream songGenreFile;

	inFile.open("input/processed.csv");
	outFile.open("output/out.csv", ofstream::trunc);

	songFile.open("output/songs.csv", ofstream::trunc);
	albumFile.open("output/albums.csv", ofstream::trunc);
	genreFile.open("output/genres.csv", ofstream::trunc);
	artistFile.open("output/artists.csv", ofstream::trunc);
	songArtistFile.open("output/songArtist.csv", ofstream::trunc);
	songGenreFile.open("output/songGenre.csv", ofstream::trunc);

	string str;
	size_t count = 0;
	size_t good_count = 0;
	size_t songArtist_count = 0;
	size_t songGenre_count = 0;

	const vector<string> artist_delimiters{ "&", "featuring", "Featuring" };

	unordered_set<string> songID;
	unordered_map<string, int> genres;
	unordered_map<string, int> artists;
	unordered_map<string, int> albums;

	auto start_time = chrono::system_clock::now();

	songArtistFile << "SongArtistID,SongID,ArtistID" << endl;
	songGenreFile << "SongGenreID,SongID,GenreID" << endl;
	songFile << "SongID,AlbumID,Name,Duration,Dancablity,Energy,Loudness,Speechiness,Acousticness,Instrumentalness,Liveliness,Valence,Popularity,Explicit" << endl;

	while (!inFile.eof()) {
		getline(inFile, str);
		count++;

		if (strman::check_empty_delimiter(str, ",") || str.length() == 0) {
			continue;
		}

		string id = strman::split_get(str, ",", 0);
		if (songID.contains(id)) {
			continue;
		}

		if (count != 1) {
			vector<string> songs = strman::split(str, ",");
			vector<string> genre_split_result = strman::read_array(songs[3]);

			for (auto& genre : genre_split_result) {
				if (genres.contains(genre)) continue;

				genres.insert({ genre, genres.size() });
			}

			for (auto& genre : genre_split_result) {
				songGenreFile << songGenre_count << ',' << id << ',' << genres[genre] << endl;
				songGenre_count++;
			}

			string artist = songs[1];
			if (artist.find_first_of('|') != string::npos) {
				continue;
			}
			else {
				vector<string> artist_split_result = strman::split(artist, artist_delimiters);

				for (auto& artist_name : artist_split_result) {
					if (artists.contains(artist_name)) continue;

					artists.insert({ artist_name, artists.size() });
				}

				for (auto& artist_name : artist_split_result) {
					songArtistFile << songArtist_count << ',' << id << ',' << artists[artist_name] << endl;
					songArtist_count++;
				}
			}

			string album = songs[8];
			if (!albums.contains(album)) {
				albums.insert({ album, albums.size() });
			}

			songFile 
				<< id << ',' 
				<< albums[album] << ','
				<< songs[2] << ','
				<< songs[6] << ','
				<< songs[9] << ','
				<< songs[10] << ','
				<< songs[12] << ','
				<< songs[14] << ','
				<< songs[15] << ','
				<< songs[16] << ','
				<< songs[17] << ','
				<< songs[18] << ','
				<< songs[21] << ','
				<< songs[7] << endl;
		}

		outFile << str << endl;

		songID.insert(id);
		good_count++;
	}

	// Post processing

	genreFile << "GenreID,GenreName" << endl;
	for (auto& pair : genres) {
		genreFile << pair.second << ',' << pair.first << endl;
	}

	artistFile << "ArtistID,ArtistName" << endl;
	for (auto& pair : artists) {
		artistFile << pair.second << ',' << pair.first << endl;
	}

	albumFile << "AlbumID,Name" << endl;
	for (auto& pair : albums) {
		albumFile << pair.second << ',' << pair.first << endl;
	}


	// Report

	chrono::duration<float, milli> duration;
	duration = chrono::system_clock::now() - start_time;

	printf("%u rows processed (%.3f s).\n", (unsigned)count, duration.count() * 0.001f);
	printf("Total rows in output: %u\n", (unsigned)good_count);

	inFile.close();
	outFile.close();
	songFile.close();
	genreFile.close();
	artistFile.close();
	songArtistFile.close();
	songGenreFile.close();
}