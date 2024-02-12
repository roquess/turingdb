import clsx from 'clsx'

import { NodePropertyValue, propertyKeyToString } from '@/components/visualizer/common/properties'
import AppExpandable from '@/components/base/AppExpandable'
import AppNodeSidebarCard from './AppNodeSidebarCard'
import AppCollapseTrigger from './AppCollapseTrigger'

function NodePropertiesListItem({ type, value }: { type: string; value: any }) {
  return (
    <li>
      <p className="mb-1 text-xs font-medium leading-[1.16] text-content-tertiary">
        {propertyKeyToString(type)}
      </p>
      <p
        className="text-sm font-medium leading-[1.43]"
        style={{
          wordBreak: type === 'schemaClass' ? 'break-all' : undefined,
        }}
      >
        <NodePropertyValue value={value} valueKey={type} />
      </p>
    </li>
  )
}

export default function AppNodePropertiesList({ properties }: { properties: Record<string, any> }) {
  const propertiesWithoutText = Object.fromEntries(
    Object.entries(properties).filter(([key]) => key !== 'text')
  )
  const textProperty = properties.text
  const isExpandable = Object.keys(propertiesWithoutText).length > 6
  const propertiesEntries = Object.entries(propertiesWithoutText)
  const preview = propertiesEntries.slice(0, 6)
  const rest = propertiesEntries.slice(6)

  return (
    <AppNodeSidebarCard className="!px-0" headerClassName="px-4" title="Properties">
      {preview.length > 0 && (
        <div className="px-4">
          <ul className="grid grid-cols-2 gap-4">
            {preview.map(([key, value]) => (
              <NodePropertiesListItem key={key} type={key} value={value} />
            ))}
          </ul>
          {isExpandable && (
            <AppExpandable
              trigger={({ isExpanded, setIsExpanded }) => (
                <AppCollapseTrigger onClick={() => setIsExpanded(!isExpanded)}>
                  {rest.length} properties
                </AppCollapseTrigger>
              )}
              hideTriggerOnExpand
            >
              <ul className="mt-4 grid grid-cols-2 gap-4">
                {rest.map(([key, value]) => (
                  <NodePropertiesListItem key={key} type={key} value={value} />
                ))}
              </ul>
            </AppExpandable>
          )}
        </div>
      )}
      {textProperty && (
        <div className={clsx('mt-4 border-grey-700 px-4 pt-2', { 'border-t': preview.length })}>
          <div className="mb-3 flex min-h-[2rem] items-center">
            <p className="text-xs font-bold uppercase leading-[1.66] tracking-[0.06em] text-content-primary">
              {propertyKeyToString('text')}
            </p>
          </div>
          <p className="text-sm font-medium leading-[1.43] text-content-secondary">
            <NodePropertyValue value={textProperty} valueKey="text" />
          </p>
        </div>
      )}
    </AppNodeSidebarCard>
  )
}
