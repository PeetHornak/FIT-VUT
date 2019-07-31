from selenium import webdriver
import re
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


browser = webdriver.Firefox()
base_url = 'http://mys01.fit.vutbr.cz:8021'
# browser.get(base_url)
# browser.implicitly_wait(20)
# browser.get('http://mys01.fit.vutbr.cz:8021/index.php?route=account/login')
# browser.find_element_by_xpath('//*[@id="input-email"]').send_keys("a@b.com")
# browser.find_element_by_xpath('//*[@id="input-password"]').send_keys("0123")
# browser.find_element_by_xpath('/html/body/div[2]/div/div/div/div[2]/div/form/input').click()
# browser.get('http://mys01.fit.vutbr.cz:8021/index.php?route=checkout/cart')
# url = browser.current_url
# browser.get('http://mys01.fit.vutbr.cz:8021/index.php?route=product/product&product_id=33')
# browser.find_element_by_xpath('//*[@id="button-cart"]').click()
# browser.get(url)
# browser.find_element_by_xpath('/html/body/div[2]/div/div/div[3]/div[2]/a').click()
# def clickme():
#     buttons = browser.find_elements_by_class_name('btn-primary')
#     for button in buttons:
#         if button.is_enabled() and button.is_displayed():
#             button.click()
#             break
#
# for i in range(2, 6):
#     browser.find_element_by_xpath(
#         '/html/body/div[2]/div/div/div/div[{}]/div[1]/h4/a'.format(i)).click()
#     clickme()
#     print(browser.find_element_by_xpath(
#         '/html/body/div[2]/div/div/div/div[{}]/div[1]/h4/a'.format(i)).is_displayed())
# a = browser.find_element_by_xpath('/html/body/div[2]/div/div/div/div[5]/div[1]/h4/a').is_displayed()
# print(a)

browser.get('http://mys01.fit.vutbr.cz:8021/admin/')
browser.find_element_by_xpath('//*[@id="input-username"]').send_keys("admin")
browser.find_element_by_xpath('//*[@id="input-password"]').send_keys("admin")
browser.find_element_by_xpath('/html/body/div/div/div/div/div/div/div[2]/form/div[3]/button').click()

url = browser.current_url
token = re.search(r'&token=\w+', url)
browser.get(
        'http://mys01.fit.vutbr.cz:8021/admin/index.php?route=catalog/product/edit&{}&product_id=30'.format(
                token.string))

browser.find_element_by_xpath(
        '/html/body/div[1]/div/div[2]/div/div[2]/form/ul/li[8]').click()

browser.find_element_by_xpath().is_displayed()

# browser.find_element_by_xpath('/html/body/div[1]/div/div[2]/div[3]/div[2]/div/div[2]/table/tbody/tr[1]/td[6]/a').click()
# browser.find_element_by_xpath('/html/body/div/div/div[1]/div/div/a[3]').click()
# field = browser.find_element_by_xpath('//*[@id="input-telephone"]')
# field.clear()
# field.send_keys('123')
# buttons = browser.find_elements_by_class_name('btn-primary')
# for button in buttons:
#         if button.is_enabled() and button.is_displayed():
#                 if button.text == 'Continue':
#                         button.click()
#                         break
#
# browser.find_element_by_class_name('text-danger')


#
#
# browser.find_element_by_xpath('/html/body/div[1]/header/div/a[2]').click()
# browser.find_element_by_xpath('//*[@id="button-menu"]').click()
# status = print(browser.find_element_by_xpath('//*[@id="column-left"]').get_attribute("class"))
# if not status == "active":
#     browser.find_element_by_xpath('//*[@id="button-menu"]').click()
# browser.find_element_by_xpath('//*[@id="catalog"]').click()
# browser.find_element_by_xpath('/html/body/div/nav/ul/li[2]/ul/li[2]/a').click()
#
#
# browser.find_element_by_xpath('/html/body/div/div/div[2]/div/div[2]/form/div/table/tbody/tr[1]/td[8]/a').click()
# field = browser.find_element_by_xpath('//*[@id="input-name1"]')
# field.clear()
# field.send_keys('30 palcove jablko do kina')
# browser.find_element_by_xpath('/html/body/div[1]/div/div[1]/div/div/button').click()
# text = browser.find_element_by_xpath('/html/body/div/div/div[2]/div[2]/div[2]/form/div/table/tbody/tr[1]/td[3]').text
#
#
# browser.find_element_by_xpath('/html/body/div/div/div[2]/div/div[2]/form/div/table/tbody/tr[1]/td[8]/a').click()
# browser.find_element_by_xpath('/html/body/div[1]/div/div[2]/div/div[2]/form/ul/li[8]').click()
# browser.find_element_by_xpath(
#     '/html/body/div[1]/div/div[2]/div/div[2]/form/div/div[8]/div/table/tfoot/tr/td[2]/button')
# browser.find_element_by_xpath('/html/body/div[1]/div/div[1]/div/div/button').click()
#
# browser.find_element_by_xpath(
#     '/html/body/div/div/div[2]/div[2]/div[2]/form/div/table/tbody/tr[1]/td[1]/input').click()
# browser.find_element_by_xpath('/html/body/div/div/div[1]/div/div/button[2]').click()


# text = browser.find_element_by_xpath('//*[@id="input-name1"]').text
# if text != "30 palcove jablko do kina":
#     print("YAY")
