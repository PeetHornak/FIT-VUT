from behave import *
from selenium.common.exceptions import NoSuchElementException
import re
import time


def click_button(context, text):
    buttons = context.browser.find_elements_by_class_name('btn-primary')
    for button in buttons:
        if button.is_enabled() and button.is_displayed():
            if button.text == text:
                button.click()
                break


def set_order_details(context, number):
    to_check = ''
    if '{}/admin/index.php?route=sale/order/edit'.format(context.base_url) in context.browser.current_url:
        to_check = context.browser.find_element_by_xpath(
            '/html/body/div/div/div[2]/div/div[2]/form/ul/li[{}]'.format(number)).get_attribute('class')
    if not to_check == 'disabled active':
        url = context.browser.current_url
        token = re.search(r'&token=\w+', url)
        context.browser.get(
            '{}/admin/index.php?route=sale/order/edit{}&order_id=1'.format(context.base_url, token.group()))
        for i in range(0, 4):
            time.sleep(0.2)
            to_check = context.browser.find_element_by_xpath(
                '/html/body/div/div/div[2]/div/div[2]/form/ul/li[{}]'.format(number)).get_attribute('class')
            if to_check == 'disabled active':
                return True
            click_button(context, 'Continue')
        time.sleep(0.2)
        to_check = context.browser.find_element_by_xpath(
            '/html/body/div/div/div[2]/div/div[2]/form/ul/li[{}]'.format(number)).get_attribute('class')
        if to_check == 'disabled active':
            return True
        else:
            return False
    else:
        return True


@given('a web browser'
       ' is at admin login page')
def step(context):
    context.browser.get('{}/admin/'.format(context.base_url))


@when('admin fills all fields')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="input-username"]').send_keys("admin")
    context.browser.find_element_by_xpath('//*[@id="input-password"]').send_keys("admin")


@when('admin clicks on "Login" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div/div/div/div/div/div/div[2]/form/div[3]/button').click()


@then('admin is logged in')
def step(context):
    url = context.browser.current_url
    assert ('{}/admin'.format(context.base_url) in url)


@given('a web browser is at admins home page')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[1]/header/div/a[2]').click()


@when('admin clicks on "Catalog" button')
def step(context):
    status = context.browser.find_element_by_xpath('//*[@id="column-left"]').get_attribute("class")
    if not status == "active":
        context.browser.find_element_by_xpath('//*[@id="button-menu"]').click()
    context.browser.find_element_by_xpath('//*[@id="catalog"]').click()


@when('admin clicks on "Products" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div/nav/ul/li[2]/ul/li[2]/a').click()


@then('product list is shown')
def step(context):
    assert(context.browser.find_element_by_xpath('/html/body/div/div/div[1]/div/h1').text == 'Products')


@given('product list is shown')
def step(context):
    url = context.browser.current_url
    token = re.search(r'&token=\w+', url)
    if '{}/admin/index.php?route=catalog/product{}'.format(context.base_url, token.group()) in url:
        return
    context.browser.get(
        '{}/admin/index.php?route=catalog/product{}'.format(context.base_url, token.group()))


@when('admin click on "Edit" button')
def step(context):
    context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/div/table/tbody/tr[1]/td[8]/a').click()


@then('edit page of product is shown')
def step(context):
    assert('{}/admin/index.php?route=catalog/product/edit'.format(context.base_url) in context.browser.current_url)


@given('edit page of product is shown')
def step(context):
    if '{}/admin/index.php?route=catalog/product/edit'.format(context.base_url) in context.browser.current_url:
        assert True
    url = context.browser.current_url
    token = re.search(r'&token=\w+', url)
    context.browser.get(
        '{}/admin/index.php?route=catalog/product/edit{}&product_id=30'.format(context.base_url, token.group()))


@given('"General" button is selected')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[1]/div/div[2]/div/div[2]/form/ul/li[1]/a').click()


@when('admin changes "Product Name" field')
def step(context):
    field = context.browser.find_element_by_xpath('//*[@id="input-name1"]')
    field.clear()
    field.send_keys('30 palcove jablko do kina')


@when('admin clicks "Save" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div[1]/div/div[1]/div/div/button').click()


@then('product name is changed')
def step(context):
    text = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div[2]/div[2]/form/div/table/tbody/tr[1]/td[3]').text
    assert (text == '30 palcove jablko do kina')


@given('"Special" button is selected')
def step(context):
    context.browser.find_element_by_xpath(
        '/html/body/div[1]/div/div[2]/div/div[2]/form/ul/li[8]').click()


@when('admin clicks on "Add Special" button')
def step(context):
    buttons = context.browser.find_elements_by_class_name('btn-primary')
    for button in buttons:
        text = button.get_attribute('data-original-title')
        if text == 'Add Special':
            button.click()
            break


@then('product price is "0.0000"')
def step(context):
    text = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div[2]/div[2]/form/div/table/tbody/tr[1]/td[5]/div').text
    assert (text == '0.0000')


@when('admin clicks on "View" button')
def step(context):
    context.browser.find_element_by_xpath(
        '/html/body/div[1]/div/div[2]/div[3]/div[2]/div/div[2]/table/tbody/tr[1]/td[6]/a').click()


@then('order details are shown')
def step(context):
    assert ('{}/admin/index.php?route=sale/order/info'.format(context.base_url) in context.browser.current_url)


@given('order details are shown')
def step(context):
    url = context.browser.current_url
    token = re.search(r'&token=\w+', url)
    context.browser.get(
        '{}/admin/index.php?route=sale/order/info{}&order_id=1'.format(context.base_url, token.group()))


@when('admin clicks on "Edit" button')
def step(context):
    context.browser.find_element_by_xpath('/html/body/div/div/div[1]/div/div/a[3]').click()


@then('customer details of order are shown')
def step(context):
    assert ('{}/admin/index.php?route=sale/order/edit'.format(context.base_url) in context.browser.current_url)


@given('customer details of order are shown')
def step(context):
    url = context.browser.current_url
    token = re.search(r'&token=\w+', url)
    context.browser.get(
        '{}/admin/index.php?route=sale/order/edit{}&order_id=1'.format(context.base_url, token.group()))


@when('admin enters "1a" into "Telephone" field')
def step(context):
    field = context.browser.find_element_by_xpath('//*[@id="input-telephone"]')
    field.clear()
    field.send_keys('1a')


@when('admin clicks on "Continue" button')
def step(context):
    click_button(context, 'Continue')


@then('error message is shown')
def step(context):
    time.sleep(0.2)
    istrue = context.browser.find_element_by_class_name('text-danger').is_displayed()
    assert istrue


@when('admin enters "123" into "Telephone" field')
def step(context):
    field = context.browser.find_element_by_xpath('//*[@id="input-telephone"]')
    field.clear()
    field.send_keys('123')


@then('products details of order are shown')
def step(context):
    time.sleep(0.2)
    to_check = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/ul/li[2]').get_attribute('class')
    assert (to_check == 'disabled active')


@given('products details of order are shown')
def step(context):
    assert (set_order_details(context, 2))


@when('admin enters "2" into quantity field of product')
def step(context):
    field = context.browser.find_element_by_css_selector('.btn-block > input:nth-child(1)')
    field.clear()
    field.send_keys("2")


@when('clicks on "Refresh" button')
def step(context):
    context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/div/div[2]/div[1]/table/tbody/tr/td[3]/div/span/button').click()


@then('order contains two of those products')
def step(context):
    time.sleep(0.2)
    unit_price = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/div/div[2]/div[1]/table/tbody/tr/td[4]').text
    total_price = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/div/div[2]/div[1]/table/tbody/tr/td[5]').text
    unit_price = float(re.sub(r'[^0-9.]', '', unit_price))
    total_price = float(re.sub(r'[^0-9.]', '', total_price))
    assert (unit_price * 2 == total_price)


@then('payment details of order are shown')
def step(context):
    time.sleep(0.2)
    to_check = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/ul/li[3]').get_attribute('class')
    assert (to_check == 'disabled active')


@given('payment details of order are shown')
def step(context):
    assert (set_order_details(context, 3))


@when('admin enters "1a" into "City" field')
def step(context):
    field = context.browser.find_element_by_xpath('//*[@id="input-payment-city"]')
    field.clear()
    field.send_keys('1a')


@when('admin enters "Bratislava" into "City" field')
def step(context):
    field = context.browser.find_element_by_xpath('//*[@id="input-payment-city"]')
    field.clear()
    field.send_keys('Bratislava')


@then('shipping details of order are shown')
def step(context):
    time.sleep(0.2)
    to_check = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/ul/li[4]').get_attribute('class')
    assert (to_check == 'disabled active')


@given('shipping details of order are shown')
def step(context):
    assert (set_order_details(context, 4))


@then('totals page is shown')
def step(context):
    time.sleep(0.2)
    to_check = context.browser.find_element_by_xpath(
        '/html/body/div/div/div[2]/div/div[2]/form/ul/li[5]').get_attribute('class')
    assert (to_check == 'disabled active')


@given('totals page is shown')
def step(context):
    assert (set_order_details(context, 5))


@when('admin click on "Save" button')
def step(context):
    context.browser.find_element_by_xpath('//*[@id="button-save"]').click()


@then('all previous changes are saved')
def step(context):
    time.sleep(0.2)
    try:
        context.browser.find_element_by_xpath('/html/body/div/div/div[2]/div[1]')
    except NoSuchElementException:
        assert False

    assert True
