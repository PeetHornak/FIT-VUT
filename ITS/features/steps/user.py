import time
from behave import *
from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.support.select import Select


@given('a web browser is at register page')
def step(context):
    context.browser.get('{}/index.php?route=account/register'.format(context.base_url))


@when('user fills all fields')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="input-firstname"]').send_keys('Sani')
    context.browser.find_element_by_xpath('//*[@id="input-lastname"]').send_keys('Fekete')
    context.browser.find_element_by_xpath('//*[@id="input-email"]').send_keys('a@c.com')
    context.browser.find_element_by_xpath('//*[@id="input-telephone"]').send_keys('0123')
    context.browser.find_element_by_xpath('//*[@id="input-address-1"]').send_keys('abc1')
    context.browser.find_element_by_xpath('//*[@id="input-city"]').send_keys('Dolna Hrochot')
    context.browser.find_element_by_xpath('//*[@id="input-postcode"]').send_keys('424242')
    select = Select(context.browser.find_element_by_xpath('//*[@id="input-zone"]'))
    select.select_by_index(1)
    context.browser.find_element_by_xpath('//*[@id="input-password"]').send_keys('0123')
    context.browser.find_element_by_xpath('//*[@id="input-confirm"]').send_keys('0123')
    context.browser.find_element_by_css_selector('div.pull-right:nth-child(1) > input:nth-child(2)').click()


@when('user clicks on "Register" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[2]/div/div/form/div/div/input[2]').click()


@then('user is registered')
def step(context):
    time.sleep(0.2)
    try:
        context.browser.find_element_by_xpath('/html/body/div[2]/div[1]')
    except NoSuchElementException:
        assert (context.browser.current_url == '{}/index.php?route=account/success'.format(context.base_url))

    context.browser.get('{}/index.php?route=account/logout'.format(context.base_url))


@given('a web browser is at login page')
def step(context):
    context.browser.get('{}/index.php?route=account/login'.format(context.base_url))


@when('user fills login credentials')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="input-email"]').send_keys("a@c.com")
    context.browser.find_element_by_xpath('//*[@id="input-password"]').send_keys("0123")


@when('user clicks on "Login" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[2]/div/div/div/div[2]/div/form/input').click()


@then('account page is shown')
def step(context):
    time.sleep(0.2)
    assert(context.browser.current_url == '{}/index.php?route=account/account'.format(context.base_url))


@given('a browser is at products page')
def step(context):
    context.browser.get('{}/index.php?route=product/product&product_id=33'.format(context.base_url))


@when('user clicks on "Add to Cart" button')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="button-cart"]').click()


@then('cart contains product')
def step(context):
    context.browser.find_element_by_xpath('/html/body/header/div/div/div[3]/div/button').click()
    try:
        context.browser.find_element_by_xpath('/html/body/header/div/div/div[3]/div/ul/li[1]/table')
    except NoSuchElementException:
        assert False, "Item not in basket"
    assert True


@given('cart contains product')
def step(context):
    url = context.browser.current_url
    context.browser.get('{}/index.php?route=product/product&product_id=33'.format(context.base_url))
    context.browser.find_element_by_xpath('//*[@id="button-cart"]').click()
    context.browser.get(url)


@given('a browser is at cart page')
def step(context):
    context.browser.get('{}/index.php?route=checkout/cart'.format(context.base_url))


@when('user enters "2" into "Quantity" field')
def step(context):
    field = context.browser.find_element_by_xpath('/html/body/div[2]/div/div/form/div/table/tbody/tr/td[4]/div/input')
    field.clear()
    field.send_keys("2")


@when('user clicks on "Update" button')
def step(context):
    context.browser.find_element_by_xpath(
        '/html/body/div[2]/div/div/form/div/table/tbody/tr/td[4]/div/span/button[1]/i').click()


@then('cart contains two of these products')
def step(context):
    field = context.browser.find_element_by_xpath(
        '/html/body/div[2]/div/div/form/div/table/tbody/tr/td[4]/div/input').get_attribute("value")
    assert (field == "2")


@when('user clicks on "Checkout" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[2]/div/div/div[3]/div[2]/a').click()


@then('checkout step 2 is shown')
def step(context):
    assert (context.browser.current_url == '{}/index.php?route=checkout/checkout'.format(context.base_url))


@given('checkout step {number} is shown')
def step(context, number):
    time.sleep(0.2)
    field = context.browser.find_element_by_xpath(
            '/html/body/div[2]/div/div/div/div[{}]/div[1]/h4/a'.format(number))
    if not field.get_attribute('aria-expanded') == 'true':
        field.click()


@then('checkout step after {number} is shown')
def step(context, number):
    next_num = int(number) + 1
    field = context.browser.find_element_by_xpath(
        '/html/body/div[2]/div/div/div/div[{}]/div[1]/h4/a'.format(next_num))
    text = field.get_attribute('aria-expanded')
    assert (text == 'true')


@when('user clicks on "Continue" button')
def step(context):
    time.sleep(0.2)
    buttons = context.browser.find_elements_by_class_name('btn-primary')
    for button in buttons:
        if button.is_enabled() and button.is_displayed():
            button.click()
            break


@then('warning message is shown')
def step(context):
    time.sleep(0.2)
    assert(context.browser.find_element_by_xpath(
        '/html/body/div[2]/div/div/div/div[5]/div[2]/div/div[1]').is_displayed())


@when('user clicks on "I have read and agree to the Terms & Conditions" box')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[2]/div/div/div/div[5]/div[2]/div/div[3]/div/input[1]').click()


@when('user clicks on "Confirm Order" button')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="button-confirm"]').click()


@then('order confirmation is shown')
def step(context):
    time.sleep(0.2)
    assert(context.browser.current_url == "{}/index.php?route=checkout/success".format(context.base_url))
