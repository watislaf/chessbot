import pyautogui


class MouseController:
    @staticmethod
    def drag(start_x, start_y, end_x, end_y, fast=0.5):
        pyautogui.moveTo(start_x, start_y, fast, pyautogui.easeInOutBack)
        pyautogui.drag(end_x - start_x, end_y - start_y, fast, pyautogui.easeInOutBack)