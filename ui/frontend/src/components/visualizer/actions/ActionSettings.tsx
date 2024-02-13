import AppButton from '@/components/base/AppButton'
import AppTooltip from '@/components/base/AppTooltip'
import SettingsDialog from '@/visualizer/components/SettingsDialog'
import IconCells from '@/assets/icons/cells.svg'
import AppDialog from '@/components/base/AppDialog'
import { Checkbox, FormGroup, Slider } from '@blueprintjs/core'
import NodeFilters from '@/visualizer/components/SettingsDialog/NodeFilters'
import LockMenu from '@/visualizer/components/SettingsDialog/LockMenu'

export default function ActionSettings() {
  return (
    <SettingsDialog
      trigger={({ open }) => (
        <AppTooltip label="Settings" placement="bottom">
          <AppButton className="w-8" icon={<IconCells />} onClick={open} />
        </AppTooltip>
      )}
      dialog={({
        isOpen,
        onClose,
        centerOnDoubleClicked,
        edgeLengthVal,
        labelValues,
        max,
        min,
        nodeSpacing,
        LENGTH_RATIO,
      }) => (
        <AppDialog
          isOpen={isOpen}
          title="Canvas settings"
          bodyClassName="overflow-hidden flex flex-col px-0 h-[60vh]"
          onClose={onClose}
        >
          <div className="px-5">
            <FormGroup className="app-form-group" label="Node spacing">
              <Slider
                className="app-slider"
                min={min}
                labelValues={labelValues}
                max={max}
                initialValue={nodeSpacing.value}
                value={nodeSpacing.value}
                onRelease={nodeSpacing.onRelease}
                onChange={nodeSpacing.onChange}
              />
            </FormGroup>

            <FormGroup className="app-form-group" label="Edge length">
              <Slider
                className="app-slider"
                min={min}
                labelValues={labelValues}
                max={max}
                initialValue={edgeLengthVal.value * LENGTH_RATIO}
                value={edgeLengthVal.value * LENGTH_RATIO}
                onRelease={edgeLengthVal.onRelease}
                onChange={(v) => edgeLengthVal.onChange(v / LENGTH_RATIO)}
              />
            </FormGroup>

            <NodeFilters
              tooltip={({ content, children }) => (
                <AppTooltip label={content} className="w-max">
                  {children}
                </AppTooltip>
              )}
              checkbox={({ children, ...rest }) => (
                <Checkbox {...rest} className="app-checkbox">
                  {children}
                </Checkbox>
              )}
            />
            <LockMenu
              className="mb-2 text-content-primary"
              selectClassName="app-select-popover"
              selectPopoverProps={{
                popoverClassName: 'app-popover app-select-popover',
              }}
              tooltip={({ content, children }) => (
                <AppTooltip label={content} className="w-max">
                  {children}
                </AppTooltip>
              )}
              button={({ children, ...rest }) => <AppButton {...rest}>{children}</AppButton>}
            />
            <AppTooltip label="Double clicking a node centers the canvas onto it" className="w-max">
              <Checkbox
                className="app-checkbox"
                label="Center on double click"
                checked={centerOnDoubleClicked.value}
                onChange={(e) => {
                  centerOnDoubleClicked.onChange(e.target.checked)
                  centerOnDoubleClicked.onRelease()
                }}
              />
            </AppTooltip>
          </div>
        </AppDialog>
      )}
    />
  )
}
