import pyautogui


class MouseController:
    def click(self, start_x, start_y, fast=0.0):
        pyautogui.moveTo(start_x, start_y, fast, pyautogui.easeInOutBack)
        pyautogui.click()

    def drag(self, start_x, start_y, end_x, end_y, fast=0.0):
        pyautogui.moveTo(start_x, start_y, fast, pyautogui.easeInOutBack)
        pyautogui.drag(end_x - start_x, end_y - start_y, fast,
                       pyautogui.easeInOutBack)
