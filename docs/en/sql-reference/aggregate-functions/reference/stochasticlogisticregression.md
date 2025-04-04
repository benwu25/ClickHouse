---
description: 'This function implements stochastic logistic regression. It can be used
  for binary classification problem, supports the same custom parameters as stochasticLinearRegression
  and works the same way.'
sidebar_position: 193
slug: /sql-reference/aggregate-functions/reference/stochasticlogisticregression
title: 'stochasticLogisticRegression'
---

# stochasticLogisticRegression

This function implements stochastic logistic regression. It can be used for binary classification problem, supports the same custom parameters as stochasticLinearRegression and works the same way.

### Parameters {#parameters}

Parameters are exactly the same as in stochasticLinearRegression:
`learning rate`, `l2 regularization coefficient`, `mini-batch size`, `method for updating weights`.
For more information see [parameters](../reference/stochasticlinearregression.md/#parameters).

```text
stochasticLogisticRegression(1.0, 1.0, 10, 'SGD')
```

**1.** Fitting

<!-- -->

    See the `Fitting` section in the [stochasticLinearRegression](/sql-reference/aggregate-functions/reference/stochasticlinearregression) description.

    Predicted labels have to be in \[-1, 1\].

**2.** Predicting

<!-- -->

    Using saved state we can predict probability of object having label `1`.

    ```sql
    WITH (SELECT state FROM your_model) AS model SELECT
    evalMLMethod(model, param1, param2) FROM test_data
    ```

    The query will return a column of probabilities. Note that first argument of `evalMLMethod` is `AggregateFunctionState` object, next are columns of features.

    We can also set a bound of probability, which assigns elements to different labels.

    ```sql
    SELECT ans < 1.1 AND ans > 0.5 FROM
    (WITH (SELECT state FROM your_model) AS model SELECT
    evalMLMethod(model, param1, param2) AS ans FROM test_data)
    ```

    Then the result will be labels.

    `test_data` is a table like `train_data` but may not contain target value.

**See Also**

- [stochasticLinearRegression](/sql-reference/aggregate-functions/reference/stochasticlogisticregression)
- [Difference between linear and logistic regressions.](https://stackoverflow.com/questions/12146914/what-is-the-difference-between-linear-regression-and-logistic-regression)
