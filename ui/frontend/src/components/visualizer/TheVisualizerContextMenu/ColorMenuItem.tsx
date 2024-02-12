import { useVisualizerContext } from '@/visualizer'
import AppPopoverMenuItem, { AppPopoverMenuItemProps } from '@/components/base/AppPopoverMenuItem'
import { COLOR_MODES } from '@/visualizer/constants'

interface Props extends AppPopoverMenuItemProps {
  elGroup: 'nodes' | 'edges'
  colorMode: (typeof COLOR_MODES)[keyof typeof COLOR_MODES]
  colorData?: any
}

export default function ColorMenuItem(props: Props) {
  const { text, icon, elGroup, colorMode, colorData: originalColorData = {} } = props
  let colorData = { ...originalColorData }
  const vis = useVisualizerContext()
  const cy = vis.cy()

  return (
    <AppPopoverMenuItem
      text={text}
      icon={icon}
      onClick={() => {
        const callbacks = vis.callbacks()
        if (!callbacks || !cy) {
          throw new Error('Init error')
        }

        const groupEls = cy[elGroup]()

        const selectedElements = groupEls.filter((n: any) => n.selected())
        const elements = selectedElements.length === 0 ? groupEls : selectedElements

        switch (colorMode) {
          case COLOR_MODES.DiscreteProperty: {
            const set = new Set(groupEls.map((e) => e.data().properties[colorData.propName]))
            const setArr = Array.from(set)
            colorData = {
              ...colorData,
              propValues: [...setArr],
            }
            break
          }

          case COLOR_MODES.GradientProperty: {
            colorData = {
              ...colorData,
              propValues: Object.fromEntries(
                elements
                  .filter((e: any) => !isNaN(parseFloat(e.data().properties[colorData.propName])))
                  .map((e) => [e.id(), parseFloat(e.data().properties[colorData.propName])])
              ),
            }
            colorData.min = Math.min.apply(Math, Object.values(colorData.propValues))
            colorData.max = Math.max.apply(Math, Object.values(colorData.propValues))
            break
          }

          default:
            break
        }

        const setColorMode =
          elGroup === 'nodes' ? callbacks.setNodeColorMode : callbacks.setEdgeColorMode

        setColorMode(
          //  @ts-expect-error
          colorMode,
          elements.map((n) => n.id()),
          colorData
        )
      }}
    />
  )
}
