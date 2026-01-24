# KerConv
Kernel creation and convolution DLL

Supported types: Sobel, Scharr, Gaussian, Median, Shift

The Scahrr kernel is made based on the Sobel kernel but with stronger contrast

```python
import kerconv as kc

img = cv2.imread(os.path.join(os.path.dirname(__file__), "testimg.jpg"))

dest_display=kc.filter_image(img,kc.KernelType.Sobel,kc.KernelVariation.Vertical,3)

cv2.imshow("Result", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

