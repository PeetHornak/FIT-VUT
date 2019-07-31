Feature: Buy product

  Scenario: Register
    Given a web browser is at register page
    When user fills all fields
    And user clicks on "Register" button
    Then user is registered

  Scenario: Log in
    Given a web browser is at login page
    When user fills login credentials
    And user clicks on "Login" button
    Then account page is shown

  Scenario: Add to cart
    Given a browser is at products page
    When user clicks on "Add to Cart" button
    Then cart contains product

  Scenario: Change quantity
    Given a browser is at cart page
    And cart contains product
    When user enters "2" into "Quantity" field
    And user clicks on "Update" button
    Then cart contains two of these products

  Scenario: Checkout order
    Given a browser is at cart page
    And cart contains product
    When user clicks on "Checkout" button
    Then checkout step 2 is shown

  Scenario Outline: Confirm checkout
    Given checkout step <number> is shown
    When user clicks on "Continue" button
    Then checkout step after <number> is shown

    Examples:
            | number  |
            | 2       |
            | 3       |
            | 4       |

  Scenario: Not agree to conditions
    Given checkout step 5 is shown
    When user clicks on "Continue" button
    Then warning message is shown

  Scenario: Agree to conditions
    Given checkout step 5 is shown
    When user clicks on "I have read and agree to the Terms & Conditions" box
    And user clicks on "Continue" button
    Then checkout step after 5 is shown

  Scenario: Confirm order
    Given checkout step 6 is shown
    When user clicks on "Confirm Order" button
    Then order confirmation is shown
