# Snake-Game-RL 🐍

> **Training a Reinforcement Learning agent to master Snake using n-step SARSA and Linear Function Approximation.**

This repository contains a C++ implementation of a Snake AI that learns to play through experience. This project explores the power of **Classical Reinforcement Learning**, pushing a linear model to its absolute limit to understand the importance of feature engineering and state representation.

---

## 🚀 The Approach: n-step SARSA

The agent utilizes the **n-step SARSA** (State-Action-Reward-State-Action) algorithm. This sits between TD(0) and Monte Carlo methods, allowing the agent to look $n$ steps ahead before updating its weights. This is crucial for Snake, as it helps propagate the reward signals from "eating" or "dying" back through the chain of moves more effectively.

### 🧠 The "Brain": 48 Weights, 12 Features
The agent uses a **Linear Function Approximator** to estimate the Q-value for each action. The intelligence is distributed across **48 total weights** (12 features per each of the 4 possible actions).

$$Q(s, a) \approx \sum_{i=1}^{12} w_i \cdot f_i(s)$$



---

## 🛠️ Feature Engineering (The 12 Features)

Since a linear model cannot learn complex spatial relationships on its own, the features must do the heavy lifting. Each action is evaluated based on:

1.  **Radar (4 features):** Proximity to walls or its own body in the cardinal directions.
2.  **Apple Direction (4 features):** Binary indicators for whether the apple is North, South, East, or West of the head.
3.  **Momentum (4 features):** The current heading of the snake to prevent illegal 180-degree turns into its own neck.

---

## 📈 Evolution & Lessons Learned

Training this agent provided deep insights into the "Linear Ceiling" of AI:

* **The 10M Episode Marathon:** Initial runs reached 10 million episodes. The agent mastered basic survival but struggled with the **XOR Problem**—the inability of a linear model to realize that moving toward an apple is bad if that path leads into a dead-end pocket.
* **The Breakthrough:** By balancing the **Reward Shaping** (Standardizing +/- 100 for major events and small penalties for wandering), the agent developed emergent behaviors like "Coiling" to maximize board space.
* **The Tail-Chasing Problem:** In high-density states (Snake Length > 50), the agent hits a limit because it perceives its moving tail as a static wall.



---

## ⚙️ Technical Stack

* **Language:** C++17
* **Algorithm:** n-step SARSA
* **Model:** Linear Function Approximation
* **Environment:** Custom 10x10 Snake Engine

## 🕹️ How to Run

1.  **Clone the repo:**
    ```bash
    git clone [https://github.com/AIwithMann/Snake-Game-RL.git](https://github.com/AIwithMann/Snake-Game-RL.git)
    cd Snake-Game-RL
    ```
2.  **Compile:**
    ```bash
    g++ -O3 main.cpp agent.cpp snake.cpp -o snake_rl
    ```
3.  **Run:**
    ```bash
    ./snake_rl
    ```

---

## 📓 Future Theory Phase
This project concludes the "Linear Phase." The next steps in my learning journey involve:
* **Deep Q-Networks (DQN):** Using hidden layers to solve non-linear spatial traps.
* **Search Algorithms:** Implementing BFS/A* for long-term pathing and connectivity analysis.
* **Policy Gradients:** Moving toward actor-critic methods for smoother convergence.

---
*written by Gemini Flash 3.0*
