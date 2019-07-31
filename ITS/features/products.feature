Feature: Product editing as admin

  Scenario: Log in as admin
    Given a web browser is at admin login page
    When admin fills all fields
    And admin clicks on "Login" button
    Then admin is logged in

  Scenario: View product list
    Given a web browser is at admins home page
    When admin clicks on "Catalog" button
    And admin clicks on "Products" button
    Then product list is shown

  Scenario: Edit product
    Given product list is shown
    When admin click on "Edit" button
    Then edit page of product is shown

  Scenario: Edit product name
    Given edit page of product is shown
    And "General" button is selected
    When admin changes "Product Name" field
    And admin clicks "Save" button
    Then product name is changed

  Scenario: Add special price
    Given edit page of product is shown
    And "Special" button is selected
    When admin clicks on "Add Special" button
    And admin clicks "Save" button
    Then product price is "0.0000"
