# Best Time to Buy and Sell Stock: Meetup Script

LeetCode 121, from brute force to O(n), then the follow-ups interviewers add on top (122, 188, 309, 714).

**How to read this script**

- Blockquotes are lines to say out loud.
- Code blocks are what you type on screen. The traces are written the way you'd type them as comments in the editor during an interview.
- **Ask the room** marks a place to stop and take answers before moving on.

**Samples used** (kept tiny so the traces are quick to type)

```
121 and its optimization:   prices = [7, 1, 5, 3]    (first four prices of the LeetCode example)
all the follow-ups:         prices = [1, 5, 3, 6]
```

---

## The plan

> Every LeetCode question gets the same three steps.
>
> 1. Come up with the naive solution.
> 2. Optimize the naive solution.
> 3. Yeah, that's it.

> The naive solution is not a throwaway. It proves you understand the problem, it gives you a correct answer to check against, and the fast solution almost always comes from looking at what the naive one wastes.

---

## Step 0: Read the problem, they say you should read the problem twice and also look at the constraints...

You get an array `prices`, where `prices[i]` is the price of a stock on day `i`. Choose one day to buy and a later day to sell. Return the biggest profit you can make. If there is no way to make a profit, return 0.

```
prices = [7, 1, 5, 3]   ->  4     buy at 1, sell at 5
prices = [3, 2, 1]      ->  0     price only falls, so don't trade
```

Pin these down before writing any code:

- You must buy before you sell.
- One buy and one sell, at most.
- Nobody forces you to trade, so the answer is never negative.
- `1 <= len(prices) <= 100,000`. The array is never empty, and it can be big.

**Ask the room:** "Why can't we just return `max(prices) - min(prices)`?"

```
prices = [7, 1, 5, 3]
          ^  ^
        max  min        7 - 1 = 6, but the 7 comes BEFORE the 1
```

> You'd have to sell before you buy. The order of the days matters, and that is the whole problem.

---

## Step 1: The naive solution

> Simplest thing that could possibly work: try every pair of days where the buy comes before the sell, and remember the best profit.

> Pro tip: for array questions, the naive solution is usually just nested for loops.

```python
class Solution(object):
    def maxProfit(self, prices):
        max_profit = 0
        n = len(prices)
        for i in range(n):                 # i = buy day
            for j in range(i + 1, n):      # j = sell day, always after i
                profit = prices[j] - prices[i]
                if profit > max_profit:
                    max_profit = profit
        return max_profit
```

### Simulation: naive

> `i` is the buy day and `j` is the sell day. `j` always starts one to the right of `i`.

```
prices = [7, 1, 5, 3]
          ^  ^
          i  j

max_profit = 0

i=0 (buy at 7)
    j=1   1 - 7 = -6    max_profit = 0
    j=2   5 - 7 = -2    max_profit = 0
    j=3   3 - 7 = -4    max_profit = 0

i=1 (buy at 1)
    j=2   5 - 1 =  4    max_profit = 4   <- new best
    j=3   3 - 1 =  2    max_profit = 4

i=2 (buy at 5)
    j=3   3 - 5 = -2    max_profit = 4

i=3 (buy at 3)
    no days left to sell on

return 4
```

> 6 comparisons for 4 prices. That's 3 + 2 + 1.

### Complexity: naive

```
Time:  O(n^2)   exactly n(n-1)/2 comparisons
Space: O(1)     a few variables, no matter how long the array is
```

> 4 prices cost us 6 comparisons. 100,000 prices cost about 5 billion. This is correct, and LeetCode will time it out.

---

## Step 2: Find the wasted work

**Ask the room:** "Look at `i=2`, where we buy at 5. Could that ever have won?"

> No. By then we've already seen a price of 1. Whatever day we sell on, having bought at 1 beats having bought at 5.

> Now read the trace by sell day instead of buy day. Take the last day, selling at 3. We tried three different buys for it.

```
sell at 3:
    bought at 7   ->  -4
    bought at 1   ->   2    <- the only one that could ever matter
    bought at 5   ->  -2
```

> For any sell day, the best buy is the cheapest price that came before it. And we don't need a loop to find that. As we walk left to right, we can carry it along in one variable.

```
sell at 1:   cheapest so far = 7     1 - 7 = -6
sell at 5:   cheapest so far = 1     5 - 1 =  4
sell at 3:   cheapest so far = 1     3 - 1 =  2
```

> One comparison per day. The inner loop was answering a question we already knew the answer to.

---

## Step 3: The optimal solution

> One pass, two variables: the cheapest price so far and the best profit so far. Every day we ask one question. If I sold today, having bought at the cheapest price so far, what would I make?

```python
class Solution(object):
    def maxProfit(self, prices):
        max_profit = 0           # best profit seen so far
        min_price = prices[0]    # cheapest price seen so far
        for curr_price in prices:
            max_profit = max(max_profit, curr_price - min_price)
            min_price = min(min_price, curr_price)
        return max_profit
```

### Simulation: optimal

```
prices = [7, 1, 5, 3]

max_profit = 0, min_price = 7

price 7:   7 - 7 =  0    max_profit = 0    min_price = 7
price 1:   1 - 7 = -6    max_profit = 0    min_price = 1   <- new low
price 5:   5 - 1 =  4    max_profit = 4    min_price = 1   <- new best
price 3:   3 - 1 =  2    max_profit = 4    min_price = 1

return 4
```

> Same answer as the naive solution, one line per day.

### Simulation: optimal, when there's no profit

```
prices = [3, 2, 1]

max_profit = 0, min_price = 3

price 3:   3 - 3 =  0    max_profit = 0    min_price = 3
price 2:   2 - 3 = -1    max_profit = 0    min_price = 2
price 1:   1 - 2 = -1    max_profit = 0    min_price = 1

return 0
```

> Every day is a new low, so selling never beats doing nothing. `max_profit` started at 0 and stays there, and 0 is exactly what "don't trade" means.

### Complexity: optimal

```
Time:  O(n)   one pass, constant work per day
Space: O(1)   two variables
```

**Ask the room:** "Does it matter which of the two lines inside the loop comes first?"

> No. If you update `min_price` first, then on a new-low day the profit works out to the price minus itself, which is 0, and 0 never beats anything. Buying and selling on the same day is the same as not trading.

### Edge cases to check

```
[5]           ->  0    one day, nothing to sell
[3, 2, 1]     ->  0    always falling
[1, 2, 3]     ->  2    always rising: buy first, sell last
[3, 3, 3]     ->  0    flat
[2, 9, 1, 4]  ->  7    new low arrives AFTER the best trade, answer must stay 7
```

---

## Follow-ups

> In an interview, solving 121 is the warm-up. The interviewer then changes one rule and watches how you adapt. Same three steps every time.

> I'll use one array for all of them so we can see how each rule changes the answer.

```
prices = [1, 5, 3, 6]

121  one transaction        ->  5    buy 1, sell 6
```

### Follow-up 1: as many transactions as you want (LeetCode 122)

**Rule change:** Buy and sell as many times as you like, but hold at most one share at a time.

**Naive:** On each day, try every choice (buy, sell, do nothing) with recursion. Correct, and O(2^n).

**The insight:**

> A rise over several days is the same amount of money as the daily steps added up. On `[1, 2, 3]`, buying at 1 and selling at 3 gives 2. So does (2-1) + (3-2). So we never need to find the perfect buy and sell days. Collect every upward step and skip every downward one.

```python
class Solution(object):
    def maxProfit(self, prices):
        profit = 0
        for i in range(1, len(prices)):
            if prices[i] > prices[i - 1]:
                profit += prices[i] - prices[i - 1]
        return profit
```

**Simulation:**

```
prices = [1, 5, 3, 6]

profit = 0

1 -> 5    +4    take    profit = 4
5 -> 3    -2    skip    profit = 4
3 -> 6    +3    take    profit = 7

return 7      (buy 1, sell 5, buy 3, sell 6)
```

```
Time:  O(n)
Space: O(1)
```

> This is a greedy solution: take every local win and it adds up to the global best.

---

### Follow-up 2: at most k transactions (LeetCode 188)

**Rule change:** At most `k` buy-sell pairs.

**Ask the room:** "Can we reuse the greedy from 122 and just keep the `k` biggest rises?"

```
prices = [1, 5, 3, 6], k = 1

rises:              +4 and +3
biggest rise:       4
buy 1, sell 6:      5     <- better, and it sits through the dip
```

> With a limit on transactions, sometimes the right move is to sit through a drop. Grabbing local wins no longer works, so we have to keep track of every option. That's dynamic programming.

**State:** Think of your money as starting at 0. Buying subtracts the price, selling adds it. For each transaction number `j` from 1 to `k`:

```
buy[j]  = most money you can have right after your j-th buy
sell[j] = most money you can have right after your j-th sell
```

**Each day, for each `j`:**

```
buy[j]  = max(buy[j],  sell[j-1] - price)    keep the old buy, or buy today
sell[j] = max(sell[j], buy[j] + price)       keep the old sell, or sell today
```

```python
class Solution(object):
    def maxProfit(self, k, prices):
        if not prices or k == 0:
            return 0
        buy = [float('-inf')] * (k + 1)   # -inf = "haven't bought yet"
        sell = [0] * (k + 1)
        for price in prices:
            for j in range(1, k + 1):
                buy[j] = max(buy[j], sell[j - 1] - price)
                sell[j] = max(sell[j], buy[j] + price)
        return sell[k]
```

**Simulation:**

```
prices = [1, 5, 3, 6], k = 2

            buy[1]   sell[1]   buy[2]   sell[2]
start        -inf       0       -inf       0
price 1:      -1        0        -1        0
price 5:      -1        4        -1        4
price 3:      -1        4         1        4
price 6:      -1        5         1        7

return sell[2] = 7
```

> Look at the price 3 line. `buy[2]` becomes 1: we have 4 in the bank from the first trade and we spend 3 on the second share. On the last line we sell it for 6, and 1 + 6 = 7.

**The link back to 121:**

> Now look only at the `buy[1]` and `sell[1]` columns. `buy[1]` is the negative of the cheapest price so far. `sell[1]` ends at 5, which is the 121 answer for this array. With k = 1 this code is our 121 solution. We were doing DP all along without calling it that.

```
Time:  O(n*k)
Space: O(k)
```

Side note for the keen ones: a transaction needs two days, so if `k >= n/2` the limit can never bind and you can fall back to the 122 greedy.

---

### Follow-up 3a: one-day cooldown (LeetCode 309)

**Rule change:** Unlimited transactions, but after you sell you must wait one day before buying again.

**The insight:**

> The 122 greedy breaks because selling today now affects what you're allowed to do tomorrow. So we track which situation we're in. On any day you are in exactly one of three states.

```
rest --buy--> hold --sell--> sold --wait a day--> rest

hold = you own a share
sold = you sold today, so tomorrow is a cooldown
rest = you own nothing and you're free to buy
```

```python
class Solution(object):
    def maxProfit(self, prices):
        hold = float('-inf')   # can't be holding before day 0
        sold = 0
        rest = 0
        for price in prices:
            prev_sold = sold
            sold = hold + price               # sell the share we were holding
            hold = max(hold, rest - price)    # keep holding, or buy from rest
            rest = max(rest, prev_sold)       # keep resting, or cooldown ends
        return max(sold, rest)
```

**Simulation:**

```
prices = [1, 5, 3, 6]

            hold    sold    rest
start       -inf      0       0
price 1:     -1     -inf      0
price 5:     -1       4       0
price 3:     -1       2       4
price 6:     -1       5       4

return max(sold, rest) = 5      (buy 1, sell 6)
```

> With unlimited trades this array gave 7. Here it gives 5. Look at the price 3 line to see why. We'd love to buy at 3 using the 4 we just made, but that 4 is sitting in `sold`, and you can only buy from `rest`. `rest` on the line above is still 0, so buying gives 0 - 3 = -3, which is worse than the -1 we already hold. The 4 only reaches `rest` one line later. That one-line delay is the cooldown.

```
Time:  O(n)
Space: O(1)
```

---

### Follow-up 3b: transaction fee (LeetCode 714)

**Rule change:** Unlimited transactions, but each one costs a fee.

**Ask the room:** "Why does the 122 greedy fail here?"

```
prices = [1, 5, 3, 6], fee = 3

take both rises:    (5 - 1 - 3) + (6 - 3 - 3) = 1 + 0 = 1
buy 1, sell 6:      6 - 1 - 3 = 2      <- better
```

> The greedy takes every rise, and now every one of those costs a fee. Sometimes it's cheaper to sit through the dip than to pay twice.

**State:** Two states this time.

```
cash = most money you can have while holding nothing
hold = most money you can have while holding a share
```

```python
class Solution(object):
    def maxProfit(self, prices, fee):
        cash = 0
        hold = -prices[0]      # bought on day 0
        for price in prices[1:]:
            cash = max(cash, hold + price - fee)   # do nothing, or sell and pay the fee
            hold = max(hold, cash - price)         # do nothing, or buy
        return cash
```

**Simulation:**

```
prices = [1, 5, 3, 6], fee = 3

            cash    hold
start         0      -1       (bought on day 0 at price 1)
price 5:      1      -1
price 3:      1      -1
price 6:      2      -1

return cash = 2      (buy 1, sell 6, pay 3)
```

> On the price 3 line, buying again would give 1 - 3 = -2, which is worse than the -1 we get from still holding the original share. So we keep it and sell at 6 for -1 + 6 - 3 = 2.

```
Time:  O(n)
Space: O(1)
```

---

## Recap

```
prices = [1, 5, 3, 6]

121   one transaction       ->  5
122   unlimited             ->  7
188   at most k = 2         ->  7
309   cooldown              ->  5
714   fee = 3               ->  2
```

```
      rule              approach        state you carry                time     space
121   one transaction   one pass        min_price, max_profit          O(n)     O(1)
122   unlimited         greedy          profit                         O(n)     O(1)
188   at most k         DP              buy[j], sell[j] for each j     O(n*k)   O(k)
309   cooldown          DP, 3 states    hold, sold, rest               O(n)     O(1)
714   fee               DP, 2 states    cash, hold                     O(n)     O(1)
```

> All five are the same loop. Walk through the days once and carry a small amount of state about the best you could have done so far. Changing the rules only changes what that state is.

**The process, for any question:**

1. Read the problem and pin down the rules.
2. Write the naive solution.
3. Trace it by hand on a small example.
4. Find the work it repeats or wastes.
5. Remove the waste.
6. Trace the new version on the same example and check the answers match.
7. Test the edge cases and state the complexity.
