#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

class BaseJudger {
public:
    BaseJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        answer_ = new char[strlen(answer) + 1];
        strcpy(answer_, answer);
    }

    virtual void Submit(size_t time, size_t memory, const char *output) = 0;

    size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        delete[] answer_;
    }

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer(const char *output) const {
        // the output must equal to the answer
        return strcmp(answer_, output) == 0;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            if (100 > score_) score_ = 100;
        }
    }
};

class OIJudger : public BaseJudger {
    bool submitted_;
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (submitted_) {
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    bool CheckAnswer(const char *output) const override {
        const char *p1 = answer_;
        const char *p2 = output;
        while (*p1 || *p2) {
            const char *e1 = p1;
            while (*e1 && *e1 != '\n') e1++;
            const char *e2 = p2;
            while (*e2 && *e2 != '\n') e2++;

            const char *t1 = e1;
            while (t1 > p1 && *(t1 - 1) == ' ') t1--;
            const char *t2 = e2;
            while (t2 > p2 && *(t2 - 1) == ' ') t2--;

            if (t1 - p1 != t2 - p2 || strncmp(p1, p2, t1 - p1) != 0) {
                return false;
            }

            p1 = (*e1 == '\n') ? e1 + 1 : e1;
            p2 = (*e2 == '\n') ? e2 + 1 : e2;
        }
        return true;
    }
};

class SpacialJudger : public BaseJudger {
    size_t full_score_time_;
    size_t full_score_memory_;
public:
    SpacialJudger(size_t time_limit, size_t memory_limit, size_t full_score_time, size_t full_score_memory, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (!CheckAnswer(output)) return;

        size_t time_score = 0;
        if (time <= full_score_time_) {
            time_score = 100;
        } else if (time >= time_limit_) {
            time_score = 0;
        } else {
            time_score = (size_t)(100.0 * (time_limit_ - time) / (time_limit_ - full_score_time_));
        }

        size_t memory_score = 0;
        if (memory <= full_score_memory_) {
            memory_score = 100;
        } else if (memory >= memory_limit_) {
            memory_score = 0;
        } else {
            memory_score = (size_t)(100.0 * (memory_limit_ - memory) / (memory_limit_ - full_score_memory_));
        }

        size_t final_score = (size_t)floor(time_score * memory_score / 100.0);
        if (final_score > score_) score_ = final_score;
    }
};

#endif
