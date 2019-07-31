Feature: Order editing as admin

  Scenario: Log in as admin
    Given a web browser is at admin login page
    When admin fills all fields
    And admin clicks on "Login" button
    Then admin is logged in

  Scenario: View details of order
    Given a web browser is at admins home page
    When admin clicks on "View" button
    Then order details are shown

  Scenario: Edit customers detail
    Given order details are shown
    When admin clicks on "Edit" button
    Then customer details of order are shown

  Scenario: Wrong telephone number entered
    Given customer details of order are shown
    When admin enters "1a" into "Telephone" field
    And admin clicks on "Continue" button
    Then error message is shown

  Scenario: Edit products detail
    Given customer details of order are shown
    When admin enters "123" into "Telephone" field
    And admin clicks on "Continue" button
    Then products details of order are shown

  Scenario: Change product quantity
    Given products details of order are shown
    When admin enters "2" into quantity field of product
    And clicks on "Refresh" button
    Then order contains two of those products

  Scenario: Edit payment details
    Given products details of order are shown
    When admin clicks on "Continue" button
    Then payment details of order are shown

  Scenario: Wrong city entered
    Given payment details of order are shown
    When admin enters "1a" into "City" field
    And admin clicks on "Continue" button
    Then error message is shown

  Scenario: Edit shipping details
    Given payment details of order are shown
    When admin enters "Bratislava" into "City" field
    And admin clicks on "Continue" button
    Then shipping details of order are shown

  Scenario: Edit order details
    Given shipping details of order are shown
    When admin clicks on "Continue" button
    Then totals page is shown

  Scenario: Save edited changes
    Given totals page is shown
    When admin click on "Save" button
    Then all previous changes are saved