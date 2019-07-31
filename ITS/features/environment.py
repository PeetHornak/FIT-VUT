from selenium import webdriver

def before_all(context):
    context.browser = webdriver.Remote(
            # command_executor='http://127.0.0.1:4444/wd/hub',
            command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',
            desired_capabilities=webdriver.DesiredCapabilities.FIREFOX)
    context.browser.implicitly_wait(15)
    context.base_url = 'http://mys01.fit.vutbr.cz:8021'

def after_all(context):
    context.browser.quit()
