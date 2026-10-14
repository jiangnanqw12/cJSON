#include <algorithm>
#include <chrono>
#include <cstdint>
#include <queue>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::tuple;
using std::vector;

// 任务结构体
struct Task {
    string id;
    string entity;
    int cost;
    vector<string> deps;
};

// 调度结果结构体
struct ScheduleResult {
    int makespan;
    vector<tuple<string, int, int, string>> schedule;
};

// 限时启发式：保证调度合法，但仅在达到下界时能够确认全局最优。
// 输入符合题意，耗时非负，且总耗时能用示例接口的 int 表示。
ScheduleResult scheduleTasks(int taskCount, const vector<Task>& tasks) {
    using Time = std::int64_t;
    using Clock = std::chrono::steady_clock;
    const auto deadline = Clock::now() + std::chrono::milliseconds(800);
    ScheduleResult result{0, {}};
    if (taskCount == 0) {
        return result;
    }

    std::unordered_map<string, int> taskIds;
    std::unordered_map<string, int> entityIds;
    taskIds.reserve(taskCount);
    entityIds.reserve(taskCount);
    vector<int> entity(taskCount);
    vector<Time> entityLoad(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        taskIds.emplace(tasks[i].id, i);
        const auto entry = entityIds.emplace(
            tasks[i].entity, static_cast<int>(entityIds.size()));
        entity[i] = entry.first->second;
        entityLoad[entity[i]] += tasks[i].cost;
    }
    const int entityCount = static_cast<int>(entityIds.size());

    vector<vector<int>> next(taskCount);
    vector<int> indegree(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        for (const string& dependency : tasks[i].deps) {
            next[taskIds.at(dependency)].push_back(i);
            ++indegree[i];
        }
    }

    vector<int> remaining = indegree;
    vector<int> topological;
    topological.reserve(taskCount);
    for (int i = 0; i < taskCount; ++i) {
        if (remaining[i] == 0) {
            topological.push_back(i);
        }
    }
    for (std::size_t i = 0; i < topological.size(); ++i) {
        for (int successor : next[topological[i]]) {
            if (--remaining[successor] == 0) {
                topological.push_back(successor);
            }
        }
    }

    vector<Time> tail(taskCount, 0);
    Time lowerBound = *std::max_element(entityLoad.begin(), entityLoad.end());
    for (auto it = topological.rbegin(); it != topological.rend(); ++it) {
        const int task = *it;
        Time longestSuccessor = 0;
        for (int successor : next[task]) {
            longestSuccessor = std::max(longestSuccessor, tail[successor]);
        }
        tail[task] = tasks[task].cost + longestSuccessor;
        lowerBound = std::max(lowerBound, tail[task]);
    }

    // 先完成一个线性时间的可行方案；即使后续搜索到时，也有完整结果。
    vector<Time> bestStart(taskCount, 0);
    vector<Time> release(taskCount, 0);
    vector<Time> entityAvailable(entityCount, 0);
    Time bestMakespan = 0;
    for (int task : topological) {
        bestStart[task] = std::max(release[task], entityAvailable[entity[task]]);
        const Time finish = bestStart[task] + tasks[task].cost;
        entityAvailable[entity[task]] = finish;
        bestMakespan = std::max(bestMakespan, finish);
        for (int successor : next[task]) {
            release[successor] = std::max(release[successor], finish);
        }
    }

    std::mt19937 random(2026);
    int strategy = 0;
    while (bestMakespan > lowerBound && Clock::now() < deadline) {
        vector<Time> priority(taskCount);
        for (int i = 0; i < taskCount; ++i) {
            if (strategy == 0) {
                priority[i] = tail[i];
            } else if (strategy == 1) {
                priority[i] = static_cast<Time>(next[i].size());
            } else if (strategy == 2) {
                priority[i] = -static_cast<Time>(tasks[i].cost);
            } else {
                priority[i] = random();
            }
        }
        if (strategy < 3) {
            ++strategy;
        }

        auto lowerPriority = [&](int a, int b) {
            if (priority[a] != priority[b]) {
                return priority[a] < priority[b];
            }
            return tasks[a].id > tasks[b].id;
        };
        std::priority_queue<int, vector<int>, decltype(lowerPriority)> ready(lowerPriority);
        remaining = indegree;
        std::fill(release.begin(), release.end(), 0);
        for (int i = 0; i < taskCount; ++i) {
            if (remaining[i] == 0) {
                ready.push(i);
            }
        }

        vector<vector<std::pair<Time, Time>>> occupied(entityCount);
        vector<Time> start(taskCount, 0);
        Time makespan = 0;
        int scheduled = 0;
        while (!ready.empty()) {
            if (Clock::now() >= deadline) {
                break;
            }
            const int task = ready.top();
            ready.pop();
            Time begin = release[task];
            const Time cost = tasks[task].cost;
            auto& intervals = occupied[entity[task]];

            // 依赖只需已经安排，不必已执行完。插入空闲区间也允许主动等待。
            if (cost > 0) {
                auto position = intervals.begin();
                while (position != intervals.end()) {
                    if (begin + cost <= position->first) {
                        break;
                    }
                    begin = std::max(begin, position->second);
                    ++position;
                }
                intervals.insert(position, {begin, begin + cost});
            }
            const Time finish = begin + cost;
            // 该候选已经不可能严格改善，保留之前的完整方案。
            if (finish >= bestMakespan) {
                break;
            }
            start[task] = begin;
            makespan = std::max(makespan, finish);
            ++scheduled;
            for (int successor : next[task]) {
                release[successor] = std::max(release[successor], finish);
                if (--remaining[successor] == 0) {
                    ready.push(successor);
                }
            }
        }
        if (scheduled == taskCount && makespan < bestMakespan) {
            bestMakespan = makespan;
            bestStart = std::move(start);
        }
    }

    result.makespan = static_cast<int>(bestMakespan);
    result.schedule.reserve(taskCount);
    for (int i = 0; i < taskCount; ++i) {
        result.schedule.emplace_back(tasks[i].id, static_cast<int>(bestStart[i]),
                                     static_cast<int>(bestStart[i] + tasks[i].cost),
                                     tasks[i].entity);
    }
    std::sort(result.schedule.begin(), result.schedule.end(),
              [](const auto& a, const auto& b) {
                  if (std::get<1>(a) != std::get<1>(b)) {
                      return std::get<1>(a) < std::get<1>(b);
                  }
                  return std::get<0>(a) < std::get<0>(b);
              });
    return result;
}
