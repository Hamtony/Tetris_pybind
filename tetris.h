#pragma once

#include <vector>
#include "wallkickdata.h"
#include "piecesdata.h"
#include "tetromino.h"
#include <queue>
#include <random>
#include <algorithm>
#include <iostream>
using namespace std;

class Tetris{
private:
    int score = 0;
    vector<vector<int>> board;
    bool hold_avaible;
    int hold = -1;
    array<int, 7> bag;
    vector<int> pool; 
    queue<int> nexts; 
    Tetromino current;
    int frezee_time = 12;
    int frezee_count = 0;
    int frezee_reset_time = 10;
    int frezee_reset_count = 0;
    bool just_rotated = false;
    int height = 40;
    int width = 10;
    int comming_lines = 0;    

    bool intersect(){
        for(int i = 0; i < current.getImage().size(); i++){
            for(int j = 0; j < current.getImage()[i].size(); j++){
                if(current.getImage()[i][j] != 0){
                    if(current.getY() + i >= height || current.getX() + j < 0 || current.getX() + j >= width){
                        return true;
                    }
                    if(board[current.getY() + i][current.getX() + j] != 0){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    int rand_piece(){
        if(pool.size() <= 0){
            pool = vector<int>(7);
            for(int i = 0; i < 7; i++){
                pool[i] = i;
            }
            random_device rd;
            mt19937 gen(rd());
            shuffle(pool.begin(), pool.end(), gen);
        }
        int id_piece = pool.back();
        pool.pop_back();
        return id_piece;
    }
    void update_nexts(){
        while(nexts.size() < 5){
            nexts.push(rand_piece());
        }
    }

    void new_piece(){
        //showq(nexts);
        current = Tetromino(nexts.front(), 0, (width/2)-2, height/2);
        nexts.pop();
        update_nexts();
    }
    bool check_tspin(){
        if (!just_rotated){
            return false;
        }
        else if (current.getType() != 5){
            return false;
        }
        else{
            int corners = 0;
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if((i!=1 && i == j) || (i==0 && j==2) || (i==2 && j==0)){
                        if(current.getY() + i >= height || current.getX() + j < 0 || current.getX() + j >= width){
                            corners++;
                        }
                        else if(board[current.getY() + i][current.getX() + j] != 0){
                            corners++;
                        }
                    }
                }
            }
            cout<<corners<<endl;
            if(corners >= 3){
                return true;
            }
            else{
                return false;
            }
        }
    }
    vector<int> piece_id_to_one_hot_encode(int piece){
        vector<int> one_hot_encode(7);
        for(int i = 0; i < 7; i++){
            one_hot_encode[i] = 0;
        }
        one_hot_encode[piece] = 1;
        return one_hot_encode;
    }
    void clear_lines(){
        bool tspin = check_tspin();
        int lines = 0;
        for(int i = 0; i < height; i++){
            bool line = true;
            for(int j = 0; j < width; j++){
                if(board[i][j] == 0){
                    line = false;
                }
            }
            if(line){
                lines++;
                for(int j = i; j > 0; j--){
                    for(int k = 0; k < width; k++){
                        board[j][k] = board[j-1][k];
                    }
                }
                for(int k = 0; k < width; k++){
                    board[0][k] = 0;
                }
            }
        }
        int gained_score = 0;
        if (lines > 0){
            if(tspin){
                gained_score = lines * 200;
            }
            else{
                switch(lines){
                    case 2:
                        gained_score += 100;
                        break;
                    case 3:
                        gained_score += 200;
                        break;
                    case 4:
                        gained_score += 400;
                        break;
                }
            }
        }
        score += gained_score;
    }
    void set_comming_lines() {
        if (comming_lines > 0) {
            // Create lines with random holes
            int hole = rand() % width; // Random hole position
            vector<vector<int>> new_lines(comming_lines, vector<int>(width, 1));
            for (int i = 0; i < comming_lines; ++i) {
                new_lines[i][hole] = 0;   // Leave the hole in the line
            }
            // Shift the board upwards to accommodate new lines at the bottom
            for (int i = 0; i < height - comming_lines; ++i) {
                board[i] = board[i + comming_lines];
            }
            // Add the new lines to the bottom of the board
            for (int i = height - comming_lines; i < height; ++i) {
                board[i] = new_lines[i - (height - comming_lines)];
            }
            // Reset the incoming lines counter
            comming_lines = 0;
        }
    }
    void frezee(){
        for(int i = 0; i < current.getImage().size(); i++){
            for(int j = 0; j < current.getImage()[i].size(); j++){
                if(current.getImage()[i][j] != 0){
                    board[current.getY() + i][current.getX() + j] = current.getType() + 1;
                }
            }
        }
        clear_lines();
        new_piece();
        hold_avaible = true;
        set_comming_lines();
        //check game over
        if(intersect()){
            done = true;
        }
    }

    bool landed(){
        for(int i = 0; i < current.getImage().size(); i++){
            for(int j = 0; j < current.getImage()[i].size(); j++){
                if(current.getImage()[i][j] != 0){
                    if (current.getX() + j < 0 || current.getX() + j >= width || 
                    current.getY() + i >= height - 1 || current.getY() + i < 0){
                        return true;
                    }
                    if(current.getY() + i + 1 > height - 1){
                        return true;
                    }
                    if(board[current.getY() + i +1][current.getX() + j] > 0){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool done;


public:
    Tetris(){
        cout << "loading game..." << endl;
        
        board = vector<vector<int>>(height);
        for(int i = 0; i < height; i++){
            board[i] = vector<int>(width);
        }
        update_nexts();
        new_piece();
        hold_avaible = true;
        done = false;
    }

    vector<vector<int>> getBoard(){return board;}
    Tetromino getCurrent(){return current;}
    int getHold(){return hold;}
    int getScore(){return score;}
    int getDone(){return done;}

    vector<int> getNexts(){
        queue<int> g = nexts;
        vector<int> n;
        while (!g.empty()) {
            n.push_back(g.front());
            g.pop();
        }
        return n;
    }
    void recive_lines(int lines){
        comming_lines += lines;
    }
    Tetromino shadow_image(){
        Tetromino shadow = current;
        int tempy = current.y;
        while(!intersect()){
            current.y++;
        }
        current.y--;
        shadow.y = current.y;
        current.y = tempy;
        return shadow;
    }
    void hold_piece(){
        if(hold_avaible){
            if(hold != -1){
                int temp = current.getType();
                current = Tetromino(hold, 0, (width/2)-2, height/2);
                hold = temp;
            }
            else{
                hold = current.getType();
                new_piece();
            }
            hold_avaible = false;
            just_rotated = false;
        }
    }
    void rotate(int dir){
        if(current.rotate(dir, board)) {
            if(landed()){
                frezee_count = 0;
                frezee_reset_count++;
            }
            just_rotated = true;
        }
    }
    void move(int dir){
        current.x = current.getX() + dir;
        if(intersect()){
            current.x = current.getX() - dir;
        }
        else{
            just_rotated = false;
            if(landed()){
                frezee_count = 0;
                frezee_reset_count++;
            }

        }
    }
    void drop(){
        while(!intersect()){
            current.y++;
        }
        current.y--;
        frezee_count=0;
        frezee_reset_count=0;
        frezee();
        just_rotated = false;

    }
    void soft_drop(int sdf){
        int firts_y = current.y;
        for (int i = 0; i < sdf; i++){
            current.y++;
            if (intersect()){
                current.y--;
                break;
            }
        }
        if (current.y != firts_y){
            just_rotated = false;
        }
    }
    void fall(){
        int firts_y = current.y;
        current.y++;
        if(intersect()){
            current.y--;
            if (landed()){
                frezee_count++;
                if (frezee_count >= frezee_time){
                    frezee();
                    frezee_count = 0;
                    frezee_reset_count = 0;
                }
            }
        }
        if (current.y != firts_y){
            just_rotated = false;
        }
    }

    int holes() {
        
    int holes = 0;
    for (int col = 0; col < width; ++col) {
        bool found_piece = false;
            for (int row = 0; row < height; ++row) {
                if (board[row][col] > 0) {
                    found_piece = true;
                } else if (found_piece && board[row][col] == 0) {
                    holes++;
                }
            }
        }
        return holes;
    }

    int total_height() {
        int sum_height = 0;

        for (int col = 0; col < width; ++col) {
            int row = 0;
            while (row < height && board[row][col] == 0) {
                row++;
            }
            int height_col = height - row;
            sum_height += height_col;
        }
        return sum_height;
    }

    int bumpiness() {
        int total_bumpiness = 0;
        vector<int> min_ys(width, height);

        // Calculate the minimum y (height) for each column
        for (int col = 0; col < width; ++col) {
            int row = 0;
            while (row < height && board[row][col] == 0) {
                row++;
            }
            min_ys[col] = row;
        }

        // Calculate bumpiness between adjacent columns
        for (int col = 0; col < width - 1; ++col) {
            total_bumpiness += abs(min_ys[col] - min_ys[col + 1]);
        }
        return total_bumpiness;
    }

    vector<int> get_state() {
        vector<int> state;

        vector<int> nxts = getNexts();
        for(int i = 0; i < nxts.size(); i++) {
            vector<int> p = piece_id_to_one_hot_encode(nxts[i]);
            for(int j = 0; j < p.size(); j++) {
                state.push_back(p[j]);
            }
        }

        vector<int> hold_ohe;
        if (hold == -1){
            for(int i = 0; i < 7; i++) {
                state.push_back(0);
            }
        }
        else{
            hold_ohe = piece_id_to_one_hot_encode(hold);
            for(int i = 0; i < hold_ohe.size(); i++) {
                state.push_back(hold_ohe[i]);
            }
        }

        vector<int> current_ohe = piece_id_to_one_hot_encode(current.getType());
        for(int i = 0; i < current_ohe.size(); i++) {
            state.push_back(current_ohe[i]);
        }
        state.push_back(current.getX());
        state.push_back(current.getY());
        state.push_back(current.getRotation());

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (board[i][j] > 0) {
                    state.push_back(1);
                }
                else {
                    state.push_back(0);
                }                
            }
        }
        state.push_back(holes());
        state.push_back(total_height());
        state.push_back(bumpiness());
        return state;
    }

};